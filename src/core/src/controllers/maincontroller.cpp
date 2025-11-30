#include "controllers/maincontroller.hpp"


MainController::MainController(QObject *parent)
	: QObject(parent), m_pluginController(PluginController(QCoreApplication::applicationDirPath().toStdString() + "/plugins", this))
	, m_sourceController(SourceController(this))
	, m_presetsController(PresetsController(QCoreApplication::applicationDirPath() + "/presets", this))
	, m_processingController(ProcessingController(this))
	, m_mountController(MountController(this))
	, m_sessionController(SessionController(m_sourceController, m_processingController, m_mountController, this))
{}

MainController::~MainController()
{}
