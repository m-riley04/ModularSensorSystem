#include "utils/session_utils.hpp"
#include "models/settings_models.hpp"
#include "features/sources/source.hpp"

const QString generateSessionDirectoryPath(const SessionSettings& props, const QString suffix)
{
	const QString outputDir = props.outputDirectory.absolutePath();

	// Check output directory
	if (!QDir(outputDir).exists()) {
		if (!QDir().mkpath(outputDir)) {
			qWarning() << "Failed to create base output directory:" << outputDir;
			return QString();
		}
	}

	const QString outputFolderPrefix = props.outputPrefix + suffix;
	const QString outputFolderPath = outputDir + "/" + outputFolderPrefix;

	// Check output directory
	if (!QDir(outputFolderPath).exists()) {
		if (!QDir().mkpath(outputFolderPath)) {
			qWarning() << "Failed to create session output directory:" << outputFolderPath;
			return QString();
		}
	}

	return outputFolderPath;
}

const QString generateSessionSourcePath(Source* src, const SessionSettings& props, const ns timestamp)
{
	auto recordableSrc = src->asRecordable();
	if (!src->asRecordable()) {
		qWarning() << "Cannot generate session source path: source is not recordable:" << QString::fromStdString(src->name());
		return QString();
	}

	QString timestampString = QString::fromStdString(std::to_string(timestamp));
	const QString outputFolderPath = QDir::cleanPath(generateSessionDirectoryPath(props, timestampString));

	// Check output folder path
	if (outputFolderPath.isEmpty()) {
		qWarning() << "Cannot generate session source path: output folder path is empty for source:" << QString::fromStdString(src->name());
		return QString();
	}

	// Sanitize the file name derived from the source display/name to avoid invalid characters
	QString baseName = QString::fromStdString(sanitizeFileNameForWindows(src->name()));

	// Avoid reserved DOS device names
	static const QStringList reserved = {
		QStringLiteral("CON"), QStringLiteral("PRN"), QStringLiteral("AUX"), QStringLiteral("NUL"),
		QStringLiteral("COM1"), QStringLiteral("COM2"), QStringLiteral("COM3"), QStringLiteral("COM4"),
		QStringLiteral("COM5"), QStringLiteral("COM6"), QStringLiteral("COM7"), QStringLiteral("COM8"), QStringLiteral("COM9"),
		QStringLiteral("LPT1"), QStringLiteral("LPT2"), QStringLiteral("LPT3"), QStringLiteral("LPT4"),
		QStringLiteral("LPT5"), QStringLiteral("LPT6"), QStringLiteral("LPT7"), QStringLiteral("LPT8"), QStringLiteral("LPT9")
	};
	if (reserved.contains(baseName.toUpper())) {
		baseName.prepend('_');
	}

	QString extension = QString::fromStdString(recordableSrc->recorderFileExtension());
	if (extension.startsWith('.')) {
		extension.remove(0, 1);
	}

	// Conservative MAX_PATH handling: keep full path well below 260
	const int maxFull = 250; // leave headroom
	QString sep = QStringLiteral("/");
	QString suffix = extension.isEmpty() ? QString() : QStringLiteral(".") + extension;
	QString candidate = outputFolderPath + sep + baseName + suffix;
	if (candidate.size() > maxFull) {
		int budget = maxFull - (outputFolderPath.size() + 1 + suffix.size());
		if (budget < 8) budget = 8; // minimum reasonable
		baseName = baseName.left(budget);
		candidate = outputFolderPath + sep + baseName + suffix;
	}

	const QString cleanedOutputFilePath = candidate.trimmed();

	if (cleanedOutputFilePath.isEmpty()) {
		qWarning() << "Cannot generate session source path: output file path is empty for source:" << QString::fromStdString(src->name());
		return QString();
	}

	return cleanedOutputFilePath;
}

std::string sanitizeFileNameForWindows(const std::string& name)
{
	// Replace characters that are invalid on Windows file systems
	// Invalid: \ / : * ? " < > |
	std::string cleaned;
	cleaned.reserve(name.size());
	for (char ch : name) {
		if (ch == '\\' || ch == '/' || ch == ':' || ch == '*' || ch == '?' || ch == '"' || ch == '<' || ch == '>' || ch == '|') cleaned.push_back('_');
		else cleaned.push_back(ch);
	}

	// Replaces spaces with underscores
	for (char& ch : cleaned) if (ch == ' ') ch = '_';

	// Remove trailing dots and spaces which are not allowed for filenames on Windows
	while (!cleaned.empty() && (cleaned.back() == '.' || cleaned.back() == ' ')) cleaned.pop_back();
	if (cleaned.empty()) cleaned = "source";
	return cleaned;
}
