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

	// Button controls
	connect(ui.buttonPrevious, &QPushButton::clicked, this, [this]() {
		int currentIndex = ui.stackedWidget->currentIndex();
		if (currentIndex > 0) {
			ui.stackedWidget->setCurrentIndex(currentIndex - 1);
		}
		});

	connect(ui.buttonNext, &QPushButton::clicked, this, [this]() {
		int currentIndex = ui.stackedWidget->currentIndex();
		if (currentIndex < ui.stackedWidget->count() - 1) {
			ui.stackedWidget->setCurrentIndex(currentIndex + 1);
		}
		});

	// Source added ui updates
	connect(sourceController, &SourceController::sourceAdded, [this](Source* source) {
		// Add source widget
		addSourceWidget(source);

		// Update UI
		updateButtonControls();
		});
	connect(sourceController, &SourceController::sourceRemoved, this, &PreviewContainerWidget::removeSourceWidget);

	connect(ui.stackedWidget, &QStackedWidget::currentChanged, this, &PreviewContainerWidget::updateButtonControls);
	connect(ui.stackedWidget, &QStackedWidget::widgetRemoved, this, &PreviewContainerWidget::updateButtonControls);
}

void PreviewContainerWidget::updateButtonControls()
{
	if (mSourcePreviewWidgets.size() <= 1) {
		ui.frameControls->setEnabled(false);
		return;
	}

	ui.frameControls->setEnabled(true);

	// Check current index
	ui.buttonPrevious->setEnabled(ui.stackedWidget->currentIndex() > 0);
	ui.buttonNext->setEnabled(ui.stackedWidget->currentIndex() < ui.stackedWidget->count() - 1);
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

	// TODO: make this a single-page system
	ui.stackedWidget->removeWidget(ui.pageMain);
	ui.stackedWidget->addWidget(widget);

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