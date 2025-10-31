#pragma once

#include <QObject>
#include <qcoreapplication.h>
#include "controllers/plugincontroller.h"
#include "controllers/datapipelinecontroller.h"
#include "controllers/presetscontroller.h"
#include "controllers/processingcontroller.h"
#include "controllers/sourcecontroller.h"
#include "controllers/mountcontroller.h"
#include "controllers/sessioncontroller.h"
#include <memory>

class DataPipelineController; // forward declare to break circular include

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	/**
	 * @brief Returns the list of backend controller pointers owned by the main controller.
	 * @return A QList containing pointers to various BackendControllerBase objects managed by this class.
	 */
	QList<BackendControllerBase*> getAllSubcontrollers() const;

	SourceController* sourceController() const { return pSourceController.get(); }
	PluginController* pluginController() const { return pPluginController.get(); }
	PresetsController* presetsController() const { return pPresetsController.get(); }
	ProcessingController* processingController() const { return pProcessingController.get(); }
	MountController* mountController() const { return pMountController.get(); }
	DataPipelineController* dataPipelineController() const { return pDataPipelineController.get(); }
	SessionController* sessionController() const { return pSessionController.get(); }

	/**
	 * @brief Checks whether the specified main controller and its associated controllers are in a valid state.
	 * Logs if any issues are found.
	 * @param controller: Pointer to the main controller to be checked.
	 * @return True if all controllers are valid, false otherwise.
	 */
	bool checkIfControllersAreOk() const;

private:
	std::unique_ptr<SourceController> pSourceController;
	std::unique_ptr<PluginController> pPluginController;
	std::unique_ptr<PresetsController> pPresetsController;
	std::unique_ptr<ProcessingController> pProcessingController;
	std::unique_ptr<MountController> pMountController;
	std::unique_ptr<DataPipelineController> pDataPipelineController;
	std::unique_ptr<SessionController> pSessionController;

signals:
	void errorOccurred(const SourceError& e);
};
