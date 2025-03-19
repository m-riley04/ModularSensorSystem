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
    // Initialize YOLO classes
    mClasses.push_back("person");
    mClasses.push_back("tvmonitor");
    mClasses.push_back("bed");
    mClasses.push_back("chair");
    mClasses.push_back("remote");
    mClasses.push_back("cell phone");

    // Initialize YOLO thread
	pYoloThread = new QThread(this);
	pYolo = new Yolo();
	pYolo->setClasses(mClasses);
    pYolo->moveToThread(pYoloThread);

    // Check when frame finishes
    connect(pYoloThread, &QThread::finished, pYolo, &QObject::deleteLater);
    pYoloThread->start();

    // Create timer to capture frames
    QTimer* trigger = new QTimer(this);
    trigger->setInterval(mCaptureIntervalMs);

    // Connect yolo signals
    connect(pYolo, &Yolo::sendDetections, this, &SinkView::receiveDetections);
    connect(this, &SinkView::sendNewFrameYolo, pYolo, &Yolo::receiveNewFrame);
    connect(trigger, &QTimer::timeout, this, &SinkView::captureFrame);

    // Start the timer
    trigger->start();
}

SinkView::FrameSizeCalculations SinkView::calculateFrameSizings()
{
	FrameSizeCalculations result;

    // Saving these as ints so there are less method calls
	int targetWidth = this->rect().width();
	int targetHeight = this->rect().height();
	int frameWidth = mFrame.size().width();
	int frameHeight = mFrame.size().height();

    // Correct for scaling
    result.widgetRatio = static_cast<double>(targetWidth) / static_cast<double>(targetHeight);
    result.frameRatio = static_cast<double>(frameWidth) / static_cast<double>(frameHeight);

    // Calculate the actual video width
    result.actualFrameWidth = result.widgetRatio > result.frameRatio ? targetHeight * result.frameRatio : targetWidth;
    result.actualFrameHeight = result.widgetRatio > result.frameRatio ? targetHeight : targetWidth / result.frameRatio;

    // Calculate offset to center video
    result.xOffset = (targetWidth - result.actualFrameWidth) / 2;
	result.yOffset = (targetHeight - result.actualFrameHeight) / 2;

    // Calculate the scaling factors
    result.widthScalingFactor = result.actualFrameWidth / frameWidth;
    result.heightScalingFactor = static_cast<double>(targetHeight) / static_cast<double>(frameHeight);

	// Return struct with all values
    return result;
}

void SinkView::paintDebugInfo(QPainter& painter, FrameSizeCalculations& sizing)
{
    if (!mFrame.isValid()) return;

    QSize frameSize = mFrame.size();
    int itemSpacing = 15;
    int xOffset = 10;
    int yOffset = 10;

	QList<FrameDebugItem> debugItems = {
		{ "Detections", mDetections.size() },
		{ "Video Width", frameSize.width() },
		{ "Video Height", frameSize.height() },
		{ "Video Ratio", sizing.frameRatio },
		{ "Widget Width", this->rect().width() },
		{ "Widget Height", this->rect().height() },
		{ "Widget Ratio", sizing.widgetRatio },
		{ "Actual Video Width", sizing.actualFrameWidth },
		{ "Actual Video Height", sizing.actualFrameHeight },
		{ "Width Scaling Factor", sizing.widthScalingFactor },
		{ "Height Scaling Factor", sizing.heightScalingFactor }
	};

    painter.setPen(QPen(Qt::white, 2));
    for (int i = 0; i < debugItems.count(); i++) {
		const auto& item = debugItems[i];
        const QString label = item.label;
		const QVariant value = item.value;

		// If the type is a double, format it to 2 decimal places
		if (value.typeId() == QMetaType::Double) {
			painter.drawText(xOffset, yOffset + (i * itemSpacing), QString("%1: %2").arg(label).arg(value.toDouble(), 0, 'f', 2));
            continue;
		}

        // Draw the label and value
		painter.drawText(xOffset, yOffset + (i * itemSpacing), QString("%1: %2").arg(label).arg(value.toString()));
	}
}

void SinkView::paintDetections(QPainter& painter, FrameSizeCalculations& sizing, QRectF& outputRect)
{
	if (!mFrame.isValid()) return;

    // Overlay detections
    painter.setPen(QPen(Qt::red, 2));
    for (const auto& detection : mDetections) {
        QRectF scaledRect(
            outputRect.x() + detection.rect.x() * sizing.widthScalingFactor,
            outputRect.y() + detection.rect.y() * sizing.heightScalingFactor,
            detection.rect.width() * sizing.widthScalingFactor,
            detection.rect.height() * sizing.heightScalingFactor
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

void SinkView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Draw video frame
    if (mFrame.isValid()) {
        QRectF targetRect = this->rect();
        QSize frameSize = mFrame.size();

        // Calculate frame sizing/scaling/positioning
        FrameSizeCalculations sizing = calculateFrameSizings();

        // Create a scaled rectangle for the video frame
        QRectF videoRect(sizing.xOffset, sizing.yOffset, sizing.actualFrameWidth, sizing.actualFrameHeight);

        // Paint actual video frame
        QVideoFrame::PaintOptions options;
        mFrame.paint(&painter, videoRect, options);

        // Paint debug info
        if (mIsDebugInfoVisible) paintDebugInfo(painter, sizing);

        // Paint detections
        if (mIsDetectionActive) paintDetections(painter, sizing, videoRect);
    }
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
	if (state != mIsDetectionActive) {
        // Change detection state
		mIsDetectionActive = state;
		emit detectionStateChanged(mIsDetectionActive);
	}
}

void SinkView::setDebugInfoVisible(bool visible)
{
	if (visible != mIsDebugInfoVisible) {
		// Change debug info visibility
        mIsDebugInfoVisible = visible;
		update(); // schedules a paint event
	}
}

void SinkView::captureFrame()
{
    if (!mIsDetectionActive) return;
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