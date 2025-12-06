#pragma once

#include <QString>
#include <QDir>
#include "controllers/loggingcontroller.hpp"

/**
 * @brief Safely recursively deletes a directory's contents based on name filters.
 * Does NOT include the . and .. entries, because that can be extremely dangerous (i already accidentally deleted the current build directory once doing this lol)
 * Logs issues if the directory does not exist or if other errors occur.
 * @param dir The directory to delete content from
 * @param nameFilters The filter for entry names in the directory. * is wildcard.
 * @param filters The extra QDir filters to filter for
 * @return True if deletion fully succeeded, false is deletion failed.
 */
static inline bool safeDeleteDirectoryContents(const QDir& dir, const QStringList& nameFilters, QDir::Filters filters = QDir::NoFilter) {
	if (!dir.exists()) {
		LoggingController::warning(QString("Cannot delete contents of directory %1: Directory does not exist").arg(dir.absolutePath()));
		return false;
	}

	bool ret = true;
	QStringList entries = dir.entryList(nameFilters, QDir::NoDotAndDotDot | filters | QDir::AllEntries);
	for (const QString& entry : entries) {
		QFileInfo entryInfo(dir.filePath(entry));

		// Directory case
		if (entryInfo.isDir()) {
			QDir subDir(entryInfo.absoluteFilePath());
			safeDeleteDirectoryContents(subDir, nameFilters);
			if (!subDir.rmdir(".")) {
				LoggingController::warning(QString("Failed to remove directory: %1").arg(subDir.absolutePath()));
				ret = false;
			}
			continue;
		}

		// File case
		if (!QFile::remove(entryInfo.absoluteFilePath())) {
			LoggingController::warning(QString("Failed to remove file: %1").arg(entryInfo.absoluteFilePath()));
			ret = false;
		}
	}

	return ret;
}