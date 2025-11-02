#include "sourcepreviewwidget.h"

SourcePreviewWidget::SourcePreviewWidget(Source* source, QWidget *parent)
	: QWidget(parent), m_source(source)
{
	ui.setupUi(this);

	this->setAttribute(Qt::WA_NativeWindow); // Ensure the widget has a native window handle for video rendering

	setAutoFillBackground(true);
}

SourcePreviewWidget::~SourcePreviewWidget()
{}
