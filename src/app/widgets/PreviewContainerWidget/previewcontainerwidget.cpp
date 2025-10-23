#include "previewcontainerwidget.h"

PreviewContainerWidget::PreviewContainerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PreviewContainerWidget::~PreviewContainerWidget()
{}

void PreviewContainerWidget::initSignals()
{
	if (pController == nullptr) {
		qDebug() << "Cannot initialize signals: main controller is null";
		return;
	}

	SourceController* sourceController = pController->sourceController();
	if (sourceController == nullptr) {
		qDebug() << "Cannot initialize signals: source controller is null";
		return;
	}

	// Device added ui updates
	connect(sourceController, &SourceController::sourceAdded, this, &PreviewContainerWidget::addSourceWidget);
	connect(sourceController, &SourceController::sourceRemoved, this, &PreviewContainerWidget::removeSourceWidget);
}


void PreviewContainerWidget::addSourceWidget(Source* source)
{
	if (source == nullptr) {
		qDebug() << "Cannot add source widget: source is null";
		return;
	}

	auto* widget = new SourcePreviewWidget(source->preview(), ui.tabDevicePreviews);

	// Null check
	if (widget == nullptr) {
		qDebug() << "Failed to create widget for source";
		return;
	}

	// Add widget to list
	mSourcePreviewWidgets.append(widget);

	// Add tab
	QString tabName = QString::fromStdString(source->name());
	ui.tabDevicePreviews->addTab(widget, tabName);

	// Emit signal
	emit sourceWidgetAdded(widget);
}

void PreviewContainerWidget::removeSourceWidget(Source* source)
{
	if (source == nullptr) {
		qDebug() << "Cannot remove source widget: source is null";
		return;
	}

	// Find the video widget for the source
	for (int i = 0; i < mSourcePreviewWidgets.size(); ++i) {
		SourcePreviewWidget* widget = mSourcePreviewWidgets[i];
		if (widget->preview()->source() == source) {
			// Remove the widget from the list and UI
			ui.tabDevicePreviews->removeTab(i);
			mSourcePreviewWidgets.removeAt(i);

			// TODO: Implement proper/more cleanup for the widget (if needed, READ QT DOCS ON QLIST/QTAB MEMORY)
			widget->deleteLater();

			emit sourceWidgetRemoved(widget); /// CONSIDER: Returning something OTHER than a pointer to the widget (since it's being deleted)
			break;
		}
	}
}