#include "sourcepreviewwidget.h"

SourcePreviewWidget::SourcePreviewWidget(Source* source, QWidget *parent)
	: QWidget(parent), m_source(source)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_NativeWindow); // Ensure the widget has a native window handle for video rendering
	setAutoFillBackground(true);

	// Check source null
	if (!m_source) return;

	// Set the source's window ID to this widget
	m_source->setWindowId(ui.sinkWidget->winId());

	// Set widget components
	ui.labelSource->setText(QString::fromStdString(m_source->name()));
}

SourcePreviewWidget::~SourcePreviewWidget()
{
	// Reset window ID
	// TODO: Is this best practice? This feels weird.
	m_source->setWindowId(0);
}
