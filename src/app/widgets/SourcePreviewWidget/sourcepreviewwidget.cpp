#include "sourcepreviewwidget.h"

SourcePreviewWidget::SourcePreviewWidget(SourcePreview* preview, QWidget *parent)
	: QWidget(parent), pPreview(preview)
{
	ui.setupUi(this);
	bool ok = connect(preview, &SourcePreview::frameReady, this, &SourcePreviewWidget::setFrame);

	qDebug() << "connect ok:" << ok
             << "widget uses preview:" << preview;

	setAutoFillBackground(true);
}

SourcePreviewWidget::~SourcePreviewWidget()
{}

void SourcePreviewWidget::setFrame(const QImage& img)
{
	if (img.isNull()) {
		qDebug() << "Received null image";
		return;
	}

	mFrame = img;
	update(); // schedule repaint (GUI thread)
}

void SourcePreviewWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    if (mFrame.isNull()) return;

    // keep aspect ratio
    QRect target = QRect(QPoint(0, 0), size()).adjusted(0, 0, -1, -1);
    QImage scaled = mFrame.scaled(target.size(), Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    p.drawImage(QPoint{ (width() - scaled.width()) / 2, (height() - scaled.height()) / 2 }, scaled);
}