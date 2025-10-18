#pragma once

#include <QObject>
#include <controllers/clipcontroller.h>
#include <controllers/recordingcontroller.h>
#include <controllers/plugincontroller.h>
#include <qcoreapplication.h>
#include <QtMultimedia>
#include <controllers/presetscontroller.h>
#include <controllers/processingcontroller.h>
#include <controllers/sourcecontroller.h>
#include "mountcontroller.h"

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
	QList<BackendControllerBase*> getAllSubcontrollers() const
	{
		return QList<BackendControllerBase*>
		{
			pSourceController.get(),
			pRecordingController.get(),
			pClipController.get(),
			pPluginController.get(),
			pPresetsController.get(),
			pProcessingController.get(),
			pMountController.get()
		};
	}

	RecordingController* recordingController() const { return pRecordingController.get(); }
	SourceController* sourceController() const { return pSourceController.get(); }
	ClipController* clipController() const { return pClipController.get(); }
	PluginController* pluginController() const { return pPluginController.get(); }
	PresetsController* presetsController() const { return pPresetsController.get(); }
	ProcessingController* processingController() const { return pProcessingController.get(); }
	MountController* mountController() const { return pMountController.get(); }

private:
	std::unique_ptr<SourceController> pSourceController;
	std::unique_ptr<RecordingController> pRecordingController;
	std::unique_ptr<ClipController> pClipController;
	std::unique_ptr<PluginController> pPluginController;
	std::unique_ptr<PresetsController> pPresetsController;
	std::unique_ptr<ProcessingController> pProcessingController;
	std::unique_ptr<MountController> pMountController;

signals:
	void errorOccurred(const SourceError& e);
};
