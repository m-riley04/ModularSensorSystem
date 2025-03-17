#include "sinkview.h"

SinkView::SinkView(QWidget *parent)
	: QWidget(parent), pSink(std::make_unique<QVideoSink>())
{
	ui.setupUi(this);

    // Connect signal
    connect(pSink.get(), &QVideoSink::videoFrameChanged, [this](QVideoFrame frame) {
        this->setVideoFrame(frame);
        });
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
    if (mFrame.isValid()) {
        QRectF targetRect = this->rect();
        QVideoFrame::PaintOptions options;
        mFrame.paint(&painter, targetRect, options);
    }
}
