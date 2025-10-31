#include "controllers/maincontroller.h"


MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pPluginController = std::make_unique<PluginController>(QCoreApplication::applicationDirPath() + "/plugins", this);
	pSourceController = std::make_unique<SourceController>(pPluginController.get(), this);
	pPresetsController = std::make_unique<PresetsController>(QCoreApplication::applicationDirPath() + "/presets", this);
	pProcessingController = std::make_unique<ProcessingController>(this);
	pMountController = std::make_unique<MountController>(this);
	pDataPipelineController = std::make_unique<DataPipelineController>(this, this);
	pSessionController = std::make_unique<SessionController>(this);
}

MainController::~MainController()
{}

QList<BackendControllerBase*> MainController::getAllSubcontrollers() const
{
	QList<BackendControllerBase*> list;
	list << pSourceController.get()
		 << pPluginController.get()
		 << pPresetsController.get()
		 << pProcessingController.get()
		 << pMountController.get()
		 << pDataPipelineController.get();
	return list;
}

bool MainController::checkIfControllersAreOk() const
{
	// Check if they are all okay just to see if we can exit early
	if (pSourceController &&
		pProcessingController &&
		pPresetsController &&
		pPluginController &&
		pDataPipelineController &&
		pSessionController && 
		pMountController)
	{
		return true;
	}

	// Otherwise, we can do more work to see which specific subcontroller(s) are not initialized
	for (BackendControllerBase* subcontroller : getAllSubcontrollers()) {
		if (!subcontroller) {
			qDebug() << "A subcontroller is not initialized:" << subcontroller->name();
			// TODO/CONSIDER: Show a message box or some other UI feedback to the user
			// TODO/CONSIDER: return early here, or continue checking all controllers? Probably easier for debugging this way.
		}
	}

	return false;
}