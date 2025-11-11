#include "controllers/maincontroller.hpp"


MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pPluginController = new PluginController(QCoreApplication::applicationDirPath() + "/plugins", this);
	pSourceController = new SourceController(pPluginController, this);
	pPresetsController = new PresetsController(QCoreApplication::applicationDirPath() + "/presets", this);
	pProcessingController = new ProcessingController(this);
	pMountController = new MountController(this);
	pSessionController = new SessionController(pSourceController, pProcessingController, pMountController, this);
}

MainController::~MainController()
{}

QList<BackendControllerBase*> MainController::getAllSubcontrollers() const
{
	QList<BackendControllerBase*> list;
	list << pSourceController
		 << pPluginController
		 << pPresetsController
		 << pProcessingController
		 << pMountController
		 << pSessionController;
	return list;
}

bool MainController::checkIfControllersAreOk() const
{
	// Check if they are all okay just to see if we can exit early
	if (pSourceController &&
		pProcessingController &&
		pPresetsController &&
		pPluginController &&
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