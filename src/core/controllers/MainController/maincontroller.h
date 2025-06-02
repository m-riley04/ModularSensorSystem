#pragma once

#include <QObject>
#include <Controllers/ClipController/clipcontroller.h>
#include <controllers/RecordingController/recordingcontroller.h>
#include <controllers/PluginController/plugincontroller.h>
#include <qcoreapplication.h>
#include <QtMultimedia>
#include <controllers/PresetsController/presetscontroller.h>
#include <controllers/ProcessingController/processingcontroller.h>
#include <controllers/SourceController/sourcecontroller.h>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

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
	void errorOccurred(const DeviceError& e);
};
