#include "sourcepreviewwidget.h"
#include <controllers/loggingcontroller.hpp>

SourcePreviewWidget::SourcePreviewWidget(Source* source, QWidget *parent)
	: QWidget(parent), m_source(source)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_NativeWindow); // Ensure the widget has a native window handle for video rendering
	setAutoFillBackground(true);

	// Check source null
	if (!m_source) {
		LoggingController::warning("Preview initialized with null source.");
		return;
	}

	// Set widget components
	ui.labelSource->setText(QString::fromStdString(m_source->name()));

	// Check if source is previewable
	auto previewable = m_source->asPreviewable();
	if (!previewable) {
		LoggingController::warning("Source is not previewable, cannot create preview widget:" + QString::fromStdString(m_source->name()));
		return;
	}

	// Set the source's window ID to this widget
	previewable->setWindowId(ui.sinkWidget->winId());
}

SourcePreviewWidget::~SourcePreviewWidget()
{
	// Reset window ID
	// TODO: Is this best practice? This feels weird.
	// UPDATED TODO: commented this out because the source is destroyed befroe this widget in normal app flow, causing a crash.
	/*if (!m_source) return;
	auto previewable = m_source->asPreviewable();
	if (!previewable) return;
	previewable->setWindowId(0);*/
}
