#pragma once

#include <QDir>
#include <QString>

struct SessionGeneralProperties {
	// TODO: add to this
};

struct SessionClippingProperties {
	bool enabled = false;
	// TODO: add more to this
};

struct SessionRecordingProperties {
	QDir outputDirectory = QDir();
	QString outputPrefix = QString();
};

struct SessionProcessingProperties {
	bool enabled = false;
	// TODO: add more to this
};

/**
 * @brief Contains the properties for a session.
 */
struct SessionProperties {
	SessionGeneralProperties generalProperties = {};
	SessionClippingProperties clippingProperties = {};
	SessionRecordingProperties recordingProperties = {};
	SessionProcessingProperties processingProperties = {};
};