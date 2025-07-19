#pragma once

#include <QObject>
#include <controllers/ClipController/clipcontroller.h>
#include <controllers/RecordingController/recordingcontroller.h>
#include <controllers/PluginController/plugincontroller.h>
#include <qcoreapplication.h>
#include <QtMultimedia>
#include <controllers/PresetsController/presetscontroller.h>
#include <controllers/ProcessingController/processingcontroller.h>
#include <controllers/SourceController/sourcecontroller.h>
#include <QtQml>

class MainController  : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int numSubControllers READ numSubControllers CONSTANT)
	Q_PROPERTY(RecordingController* recordingController READ recordingController CONSTANT)
	Q_PROPERTY(SourceController* sourceController READ sourceController CONSTANT)
	Q_PROPERTY(ClipController* clipController READ clipController CONSTANT)
	Q_PROPERTY(PluginController* pluginController READ pluginController CONSTANT)
	Q_PROPERTY(PresetsController* presetsController READ presetsController CONSTANT)
	Q_PROPERTY(ProcessingController* processingController READ processingController CONSTANT)

public:
	MainController(QObject *parent = nullptr);
	~MainController();

	int numSubControllers() const { return 6; }

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
			pProcessingController.get()
		};
	}

	RecordingController* recordingController() const { return pRecordingController.get(); }
	SourceController* sourceController() const { return pSourceController.get(); }
	ClipController* clipController() const { return pClipController.get(); }
	PluginController* pluginController() const { return pPluginController.get(); }
	PresetsController* presetsController() const { return pPresetsController.get(); }
	ProcessingController* processingController() const { return pProcessingController.get(); }

private:
	std::unique_ptr<SourceController> pSourceController;
	std::unique_ptr<RecordingController> pRecordingController;
	std::unique_ptr<ClipController> pClipController;
	std::unique_ptr<PluginController> pPluginController;
	std::unique_ptr<PresetsController> pPresetsController;
	std::unique_ptr<ProcessingController> pProcessingController;


signals:
	void errorOccurred(const SourceError& e);
};
