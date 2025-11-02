#include "previewcontainerwidget.h"
#include <utils/boost_qt_conversions.h>

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

void PreviewContainerWidget::setController(MainController* controller) {
	if (pController == controller) return;
	pController = controller;
	initSignals();
}

void PreviewContainerWidget::addSourceWidget(Source* source)
{
	if (source == nullptr) {
		qDebug() << "Cannot add source widget: source is null";
		return;
	}

	// Create new source preview widget
	auto* widget = new SourcePreviewWidget(source, this);
	if (widget == nullptr) {
		qDebug() << "Failed to create widget for source";
		return;
	}

	// Add widget to list
	mSourcePreviewWidgets.append(widget);

	// Emit signal
	emit sourceWidgetAdded(widget);
}

void PreviewContainerWidget::removeSourceWidget(QUuid id)
{
	if (id.isNull()) {
		qDebug() << "Cannot remove source widget: source id is null";
		return;
	}

	// Find the video widget for the source
	for (auto& widget : mSourcePreviewWidgets) {
		if (boostUuidToQUuid(widget->source()->uuid()) != id) {
			continue;
		}

		// Remove the widget from the list and UI
		mSourcePreviewWidgets.removeAll(widget);

		// TODO: Implement proper/more cleanup for the widget (if needed, READ QT DOCS ON QLIST/QTAB MEMORY)
		widget->deleteLater();

		emit sourceWidgetRemoved(widget); /// CONSIDER: Returning something OTHER than a pointer to the widget (since it's being deleted)
		break;
	}
}