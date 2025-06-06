#include "sourceswidget.h"

SourcesWidget::SourcesWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initWidgets();
	initSignals();
}

SourcesWidget::SourcesWidget(SourceController* sourceController, QWidget* parent)
	: QWidget(parent), pSourceController(sourceController)
{
	ui.setupUi(this);
	initWidgets();
	initSignals();
}

SourcesWidget::~SourcesWidget()
{}

void SourcesWidget::initWidgets()
{
	// Turn off controls frame visibility by default
	ui.frameControls->setVisible(mControlsVisible);
}

void SourcesWidget::initSignals()
{
	connect(ui.listSources, &QListWidget::currentItemChanged, this, &SourcesWidget::selectedSourceChanged);

	initDeviceControllerSignals();
}

void SourcesWidget::initDeviceControllerSignals()
{
	if (pSourceController == nullptr) return;

	// Disconnect previous signals
	disconnect(pSourceController, &SourceController::sourceAdded, this, nullptr);
	disconnect(pSourceController, &SourceController::sourceRemoved, this, nullptr);

	// Connect source controller signals
	connect(pSourceController, &SourceController::sourceAdded, this, [this](Source* source) {
		if (source == nullptr) return;

		// Add the source to the list
		QString listItemName = source->name() + " (" + Source::typeToString(source->type()) + ")";
		QVariant devicePtr = QVariant::fromValue(source);
		QListWidgetItem* item = new QListWidgetItem(listItemName, ui.listSources);
		item->setData(Qt::UserRole, devicePtr);
		ui.listSources->addItem(item);
		});
	connect(pSourceController, &SourceController::sourceRemoved, this, [this](Source* source) {
		if (source == nullptr) return;

		// Find source in list widget
		for (int i = 0; i < ui.listSources->count(); ++i) {
			QListWidgetItem* itm = ui.listSources->item(i);
			Source* stored = itm->data(Qt::UserRole).value<Source*>();
			if (stored == source) {
				delete ui.listSources->takeItem(i); // returns the item; we delete it
				// break; // CONSIDER: don't break so that it continues to remove that pointer (if in multiple times)
			}
		}
		});
}

void SourcesWidget::setSourceController(SourceController* sourceController)
{
	if (sourceController == nullptr) return;
	pSourceController = sourceController;

	// Connect signals
	initDeviceControllerSignals();

	emit sourceControllerChanged(sourceController);
}
