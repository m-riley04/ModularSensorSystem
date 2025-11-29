#pragma once

#include <QDir>
#include <QString>
#include <string>
#include <QCoreApplication>

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

/**
 * @brief Initializes default session properties.
 * @return 
 */
static inline SessionProperties initSessionProps(QString outputDirectoryName, QString outputPrefix) {
	// Create sessions directory if it doesn't exist
	QDir sessionsDir = QDir(QCoreApplication::applicationDirPath() + outputDirectoryName);
	if (!sessionsDir.exists()) {
		if (!QDir().mkpath(sessionsDir.absolutePath())) {
			qWarning() << "Failed to create sessions directory:" << sessionsDir.absolutePath() << "\nUsing application directory instead.";
			sessionsDir = QDir(QCoreApplication::applicationDirPath());
		}
	}

	return {
		.generalProperties = {},
		.clippingProperties = {
			.enabled = false,
		},
		.recordingProperties = {
			.outputDirectory = sessionsDir,
			.outputPrefix = outputPrefix,
		},
		.processingProperties = {
			.enabled = false,
		},
	};
}