#include "sinkview.h"

SinkView::SinkView(QWidget *parent)
	: QWidget(parent), pSink(std::make_unique<QVideoSink>())
{
	ui.setupUi(this);

    // Connect raw video frame signal
    connect(pSink.get(), &QVideoSink::videoFrameChanged, [this](QVideoFrame frame) {
        this->setVideoFrame(frame);
        });

	// Initialize YOLO
	initializeYolo();
}

SinkView::~SinkView()
{
    if (pYoloThread) {
        pYoloThread->quit();
        pYoloThread->wait();
    }
}

void SinkView::initializeYolo()
{
    // Initialize YOLO thread
	pYoloThread = new QThread(this);
	pYolo = new Yolo();
    pYolo->moveToThread(pYoloThread);

    // Check when frame finishes
    connect(pYoloThread, &QThread::finished, pYolo, &QObject::deleteLater);
    pYoloThread->start();

    // Create timer to capture frames
    QTimer* trigger = new QTimer(this);
    trigger->setInterval(captureIntervalMs);

    // Initialize YOLO classes
    mClasses.push_back("person");
    mClasses.push_back("tvmonitor");
    mClasses.push_back("bed");
    mClasses.push_back("chair");
    mClasses.push_back("remote");
    mClasses.push_back("cell phone");

    // Connect yolo signals
    connect(pYolo, &Yolo::sendDetections, this, &SinkView::receiveDetections);
    connect(this, &SinkView::sendNewFrameYolo, pYolo, &Yolo::receiveNewFrame);
    connect(trigger, &QTimer::timeout, this, &SinkView::captureFrame);

    // Start the timer
    trigger->start();
}

void SinkView::setVideoFrame(const QVideoFrame & frame)
{
    if (frame.isValid()) {
        mFrame = frame;
        update(); // schedules a paint event
    }
}

void SinkView::setDetectionState(bool state)
{
	if (state != isDetectionActive) {
        // Change detection state
		isDetectionActive = state;
		emit detectionStateChanged(isDetectionActive);
	}
}

void SinkView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    

    // Draw video frame
    if (mFrame.isValid()) {
        QRectF videoRect;
        QRectF targetRect = this->rect();
        QSize frameSize = mFrame.size();

        // Correct for scaling
		double widgetRatio = targetRect.width() / targetRect.height();
		double videoRatio = static_cast<double>(frameSize.width()) / static_cast<double>(frameSize.height());

		// Calculate the actual video width
        double actualVideoWidth;
        if (widgetRatio > videoRatio) {
            // The widget is wider than the video’s aspect ratio.
            // The video fills the widget height and its width is determined by the video aspect.
            actualVideoWidth = targetRect.height() * videoRatio;
        }
        else {
            // The widget is narrower (or equal in aspect) so the video fills the widget width.
            actualVideoWidth = targetRect.width();
        }

		// Calculate offset to center video
		int xOffset = (targetRect.width() - actualVideoWidth) / 2;

		// Calculate the scaling factor
        double widthScalingFactor = actualVideoWidth / frameSize.width();
		double heightScalingFactor = targetRect.height() / frameSize.height();
        
        // Paint actual video frame
        QVideoFrame::PaintOptions options;
        mFrame.paint(&painter, targetRect, options);

        // Paint the total number of detections in the corner
        painter.setPen(QPen(Qt::white, 2));
        painter.drawText(10, 20, QString("Detections: %1").arg(mDetections.size()));
        painter.drawText(10, 35, QString("Video Width: %1").arg(frameSize.width()));
        painter.drawText(10, 50, QString("Video Height: %1").arg(frameSize.height()));
        painter.drawText(10, 65, QString("Video Ratio: %1").arg(videoRatio));
        painter.drawText(10, 80, QString("Widget Width: %1").arg(targetRect.width()));
        painter.drawText(10, 95, QString("Widget Height: %1").arg(targetRect.height()));
        painter.drawText(10, 110, QString("Widget Ratio: %1").arg(widgetRatio));
        painter.drawText(10, 125, QString("Actual Video Width: %1").arg(actualVideoWidth));
        painter.drawText(10, 140, QString("Actual Video Height: %1").arg(targetRect.height()));
        painter.drawText(10, 155, QString("Width Scaling Factor: %1").arg(widthScalingFactor));
        painter.drawText(10, 170, QString("Height Scaling Factor: %1").arg(heightScalingFactor));

        // Overlay detections
        painter.setPen(QPen(Qt::red, 2));
        for (const auto& detection : mDetections) {
            QRectF scaledRect(
                videoRect.x() + xOffset + detection.rect.x() * widthScalingFactor,
                videoRect.y() + detection.rect.y() * heightScalingFactor,
                detection.rect.width() * widthScalingFactor,
				detection.rect.height() * heightScalingFactor
            );
            painter.drawRect(scaledRect);

            // Prepare text label with class and confidence
            QString label;
            if (!mClasses.empty() && detection.classId < mClasses.size()) {
                label = QString::fromStdString(mClasses[detection.classId]) + ": " +
                    QString::number(detection.confidence, 'f', 2);
            }
            else {
                label = QString("ID %1: %2").arg(detection.classId)
                    .arg(detection.confidence, 0, 'f', 2);
            }
            painter.drawText(scaledRect.topLeft(), label);
        }
    }
}

void SinkView::captureFrame()
{
    if (!isDetectionActive) return;
    if (mFrame.size().isEmpty()) return;

    // Convert the current video frame to a QImage and send it to YOLO.
    QImage img = mFrame.toImage();

    // Send frame
    emit sendNewFrameYolo(img);
}

void SinkView::activateYOLO() {
    QImage img = mFrame.toImage();
    emit sendNewFrameYolo(img);
}

void SinkView::receiveDetections(std::vector<Yolo::Detection> detections)
{
    mDetections = detections;
    update(); // Trigger a repaint with the new detections.
}