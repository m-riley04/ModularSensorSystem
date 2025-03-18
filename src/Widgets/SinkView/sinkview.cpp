#include "sinkview.h"

SinkView::SinkView(QWidget *parent)
	: QWidget(parent), pSink(std::make_unique<QVideoSink>()), pYolo(std::make_unique<Yolo>())
{
	ui.setupUi(this);

    // Create timer to capture frames
    QTimer* trigger = new QTimer(this);
    trigger->setInterval(30);

	// Initialize YOLO classes
    mClasses.push_back("person");
	mClasses.push_back("pencil");
	mClasses.push_back("tv");
    mClasses.push_back("pen");
	mClasses.push_back("television");

    // Connect raw video frame signal
    connect(pSink.get(), &QVideoSink::videoFrameChanged, [this](QVideoFrame frame) {
        this->setVideoFrame(frame);
        });

    // Connect yolo signals
	connect(pYolo.get(), &Yolo::sendDetections, this, &SinkView::receiveDetections);
	connect(this, &SinkView::sendNewFrameYolo, pYolo.get(), &Yolo::receiveNewFrame);
	connect(trigger, &QTimer::timeout, this, &SinkView::captureFrame);

    activateYOLO();
	trigger->start();
    
}

SinkView::~SinkView()
{}

void SinkView::setVideoFrame(const QVideoFrame & frame)
{
    if (frame.isValid()) {
        mFrame = frame;
        update(); // schedules a paint event
    }
}

void SinkView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Draw video frame
    if (mFrame.isValid()) {
        QRectF targetRect = this->rect();
        QVideoFrame::PaintOptions options;
        mFrame.paint(&painter, targetRect, options);
    }

    // Paint the total number of detections in the corner
	painter.setPen(QPen(Qt::white, 2));
	painter.drawText(10, 20, QString("Detections: %1").arg(mDetections.size()));

    // Overlay detections
    painter.setPen(QPen(Qt::red, 2));
    for (const auto& detection : mDetections) {
		painter.drawRect(detection.rect);

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
		painter.drawText(detection.rect.topLeft(), label);
    }
}

void SinkView::captureFrame()
{
    if (mFrame.size().isEmpty()) return;

    // Convert the current video frame to a QImage and send it to YOLO.
    QImage img = mFrame.toImage();
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