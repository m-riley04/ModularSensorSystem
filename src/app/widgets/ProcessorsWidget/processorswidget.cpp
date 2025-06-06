#include "processorswidget.h"

ProcessorsWidget::ProcessorsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initWidgets();
	initSignals();
}

ProcessorsWidget::~ProcessorsWidget()
{}

void ProcessorsWidget::initWidgets()
{
	// Turn off controls frame visibility by default
	ui.frameControls->setVisible(mControlsVisible);
}

void ProcessorsWidget::initSignals()
{
	if (!pController) return;

	// Connect ui signals
	connect(ui.checkboxEnable, &QCheckBox::toggled, this, &ProcessorsWidget::onEnabledChanged);
	connect(ui.buttonAdd, &QPushButton::clicked, this, &ProcessorsWidget::onAddProcessorClicked);
	connect(ui.buttonRemove, &QPushButton::clicked, this, &ProcessorsWidget::onRemoveProcessorClicked);
	connect(ui.listProcessors, &QListWidget::currentItemChanged, this, &ProcessorsWidget::onProcessorSelected);

	// Connect controller signals
	ProcessingController* processingController = pController->processingController();
	if (!processingController) return;
	connect(processingController, &ProcessingController::processorAdded, this, &ProcessorsWidget::onProcessorAdded);
	connect(processingController, &ProcessingController::processorRemoved, this, &ProcessorsWidget::onProcessorRemoved);
	connect(processingController, &ProcessingController::processingStarted, this, &ProcessorsWidget::onProcessingStarted);
	connect(processingController, &ProcessingController::processingStopped, this, &ProcessorsWidget::onProcessingStopped);

}

void ProcessorsWidget::onProcessorAdded(ProcessorBase* processor)
{
	if (!processor) return;

	// Add processor to the list widget
	QListWidgetItem* item = new QListWidgetItem(processor->objectName(), ui.listProcessors); // TODO: use more descriptive name
	item->setData(Qt::UserRole, QVariant::fromValue(processor));
	ui.listProcessors->addItem(item);

	// Select the newly added processor
	ui.listProcessors->setCurrentItem(item);
}

void ProcessorsWidget::onProcessorRemoved(ProcessorBase* processor)
{
	if (!processor) return;

	// Find the processor in the list and remove it
	for (int i = 0; i < ui.listProcessors->count(); ++i) {
		QListWidgetItem* item = ui.listProcessors->item(i);
		if (item->data(Qt::UserRole).value<ProcessorBase*>() == processor) {
			delete ui.listProcessors->takeItem(i);
			break;
		}
	}
}

void ProcessorsWidget::onEnabledChanged(bool enabled)
{
	// Enable or disable the processing controller based on the checkbox state
	pController->processingController()->setActive(enabled);
}

void ProcessorsWidget::onAddProcessorClicked()
{
	
}

void ProcessorsWidget::onRemoveProcessorClicked()
{
	
}

void ProcessorsWidget::onProcessorSelected(QListWidgetItem* current, QListWidgetItem* previous)
{
	
}

void ProcessorsWidget::onProcessingStarted()
{
	
}

void ProcessorsWidget::onProcessingStopped()
{
	
}

void ProcessorsWidget::onProcessingError(const QString& msg)
{
	
}

