#pragma once

#include <QAction>

struct SourceActions {
    QAction* openAddSource;
    QAction* openRemoveSource;
    QAction* openEditSource;
};

struct MountActions {
    QAction* openAddMount;
    QAction* openRemoveMount;
    QAction* openEditMount;
};


struct ProcessorActions {
    QAction* openAddProcessor;
    QAction* openRemoveProcessor;
    QAction* openEditProcessor;
    QAction* toggleProcessing;
};

struct SessionActions {
    QAction* toggleSession;
    QAction* restartSession;
    QAction* toggleRecording;
    QAction* clipSession;
};

struct PresetActions {
    QAction* savePreset;
    QAction* loadPreset;
    QAction* deletePreset;
    QAction* refreshPreset;
};


struct ViewActions {
    QAction* viewPresetsList;
    QAction* viewControls;
    QAction* viewMenuBar;
    QAction* viewToolbar;
};

struct MiscActions {
	QAction* openPresetsDirectory;
    QAction* openPluginsDirectory;
    QAction* openLogsDirectory;
    QAction* openAppDirectory;
	QAction* openOutputDirectory;
    QAction* openGithubRepository;
    QAction* generatePipelineDiagram;
    QAction* openAppProperties;
    QAction* openPluginDialog;
    QAction* clearLogs;
    QAction* quit;
    QAction* restart;
};

struct AppActions {
    PresetActions* presetActions;
    MountActions* mountActions;
    SourceActions* sourceActions;
    ProcessorActions* processorActions;
    SessionActions* sessionActions;
    ViewActions* viewActions;
    MiscActions* miscActions;
};