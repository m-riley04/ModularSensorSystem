#include "sourcepreviewwidget.h"

SourcePreviewWidget::SourcePreviewWidget(Source* source, QWidget *parent)
	: QWidget(parent), m_source(source)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_NativeWindow); // Ensure the widget has a native window handle for video rendering
	setAutoFillBackground(true);

	// Check source null
	if (!m_source) {
		qWarning() << "SourcePreviewWidget initialized with null source.";
		return;
	}

	// Set widget components
	ui.labelSource->setText(QString::fromStdString(m_source->name()));

	// Check if source is previewable
	if (!m_source->asPreviewable()) {
		qWarning() << "Source is not previewable, cannot create preview widget:" << QString::fromStdString(m_source->name());
		return;
	}

	// Set the source's window ID to this widget
	m_source->asPreviewable()->setWindowId(ui.sinkWidget->winId());
}

SourcePreviewWidget::~SourcePreviewWidget()
{
	// Reset window ID
	// TODO: Is this best practice? This feels weird.
	m_source->asPreviewable()->setWindowId(0);
}
