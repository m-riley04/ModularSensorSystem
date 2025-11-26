#include "previewcontainerwidget.h"

PreviewContainerWidget::PreviewContainerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_gridWidgets.push_back(ui.pageMain); // TODO/FIX: does this need to be a cast?
}

PreviewContainerWidget::~PreviewContainerWidget()
{}

void PreviewContainerWidget::initSignals()
{
	if (m_controller == nullptr) {
		qDebug() << "Cannot initialize signals: main controller is null";
		return;
	}

	SourceController& sourceController = m_controller->sourceController();

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
	connect(&sourceController, &SourceController::sourceAdded, this, &PreviewContainerWidget::addSourceWidget);
	connect(&sourceController, &SourceController::sourceRemoved, this, &PreviewContainerWidget::removeSourceWidget);
	
	// Page changed ui updates
	connect(ui.stackedWidget, &QStackedWidget::currentChanged, this, &PreviewContainerWidget::updateButtonControls);
	connect(ui.stackedWidget, &QStackedWidget::widgetRemoved, this, &PreviewContainerWidget::updateButtonControls);
}

void PreviewContainerWidget::updateButtonControls()
{
	if (ui.pageMain->widgets().size() <= 1) {
		ui.frameControls->setEnabled(false);
		return;
	}

	ui.frameControls->setEnabled(true);

	// Check current index
	ui.buttonPrevious->setEnabled(ui.stackedWidget->currentIndex() > 0);
	ui.buttonNext->setEnabled(ui.stackedWidget->currentIndex() < ui.stackedWidget->count() - 1);
}

void PreviewContainerWidget::setController(MainController* controller) {
	if (m_controller == controller) return;
	m_controller = controller;
	initSignals();
}

void PreviewContainerWidget::addSourceWidget(Source* source)
{
	if (!source) {
		qWarning() << "Cannot add source widget: source is null";
		return;
	}

	// Create new source preview widget
	auto* widget = new SourcePreviewWidget(source, this);
	if (!widget) {
		qWarning() << "Failed to create widget for source";
		return;
	}

	// TODO: make this system add pages when too many sources are added
	ui.pageMain->addWidgetToGrid(widget);

	// Update UI
	updateButtonControls();
}

void PreviewContainerWidget::removeSourceWidget(QUuid id)
{
	if (id.isNull()) {
		qDebug() << "Cannot remove source widget: source id is null";
		return;
	}

	// Find and remove the source preview widget
	for (QWidget* widget : ui.pageMain->widgets()) {
		auto* srcWidget = dynamic_cast<SourcePreviewWidget*>(widget);

		if (srcWidget == nullptr) {
			continue;
		}

		if (boostUuidToQUuid(srcWidget->source()->uuid()) != id) {
			continue;
		}

		ui.pageMain->removeWidgetFromGrid(widget);
		widget->deleteLater();
		return;
	}


	// Update UI
	updateButtonControls();
}