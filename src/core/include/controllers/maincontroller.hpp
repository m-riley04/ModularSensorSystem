#pragma once

#include <QObject>
#include <QThread>
#include <qcoreapplication.h>
#include <memory>
#include "controllers/plugincontroller.hpp"
#include "controllers/presetscontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "controllers/sessioncontroller.hpp"

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

	SourceController* sourceController() const { return pSourceController; }
	PluginController* pluginController() const { return pPluginController; }
	PresetsController* presetsController() const { return pPresetsController; }
	ProcessingController* processingController() const { return pProcessingController; }
	MountController* mountController() const { return pMountController; }
	SessionController* sessionController() const { return pSessionController; }

	/**
	 * @brief Checks whether the specified main controller and its associated controllers are in a valid state.
	 * Logs if any issues are found.
	 * @param controller: Pointer to the main controller to be checked.
	 * @return True if all controllers are valid, false otherwise.
	 */
	bool checkIfControllersAreOk() const;

private:
	// NOTE: These were previously unique_ptrs.
	// I favored for Qt dealing with object ownership and parent-child relationships.
	// It was also causing issues with double deletion side-effects.
	SourceController* pSourceController;
	PluginController* pPluginController;
	PresetsController* pPresetsController;
	ProcessingController* pProcessingController;
	MountController* pMountController;
	SessionController* pSessionController;

};
