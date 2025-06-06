#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pPluginController = std::make_unique<PluginController>(QCoreApplication::applicationDirPath() + "/plugins", this);
	pSourceController = std::make_unique<SourceController>(pPluginController.get(), this);
	pRecordingController = std::make_unique<RecordingController>(pSourceController.get(), this);
	pPresetsController = std::make_unique<PresetsController>(QCoreApplication::applicationDirPath() + "/presets", this);
	pProcessingController = std::make_unique<ProcessingController>(this);
	pClipController = std::make_unique<ClipController>(QDir::currentPath() + "/output/clips/", this); // TODO: Make this a configurable path (and just correct)
}

MainController::~MainController()
{}
