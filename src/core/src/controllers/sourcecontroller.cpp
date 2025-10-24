#include "controllers/sourcecontroller.h"
#include <utils/boost_qt_conversions.h>

SourceController::SourceController(PluginController* pluginController, QObject *parent)
	: BackendControllerBase("SourceController", parent), pPluginController(pluginController), pRecordingSession(nullptr)
{}

SourceController::~SourceController()
{}

Source* SourceController::byId(const QUuid & id) const
{
	return mSourcesById.value(id);
}

void SourceController::openSources()
{
	for (Source* source : mSources) {
		if (source) {
			source->open();
		}
	}

	mState = OPENED;
	emit stateChanged(OPENED);
}

void SourceController::startSources()
{
	for (Source* source : mSources) {
		if (source) {
			source->start();
		}
	}

	mState = STARTED;
	emit stateChanged(STARTED);
}

void SourceController::stopSources()
{
	for (Source* source : mSources) {
		if (source) {
			source->stop();
		}
	}

	mState = STOPPED;
	emit stateChanged(STOPPED);
}

void SourceController::closeSources()
{
	for (Source* source : mSources) {
		if (source) {
			source->close();
		}
	}

	mState = CLOSED;
	emit stateChanged(CLOSED);
}

void SourceController::restartSources()
{
	for (Source* source : mSources) {
		if (source) {
			source->restart();
		}
	}

	emit sourcesRestarted();
}

Source* SourceController::addSource(ISourcePlugin* plugin, SourceInfo info) {
	if (!plugin) {
		emit errorOccurred({
			"Cannot add source: plugin is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});
		qWarning() << "Plugin for source is null";
		return nullptr;
	}
	
	auto source = plugin->createSource(info.id, this);

	mSources.append(source);
	QUuid uid = boostUuidToQUuid(source->uuid());
	mSourcesById[uid] = source;

	// Connect errors
	connect(source, &Source::errorOccurred, this, &SourceController::errorOccurred);

	emit sourceAdded(source);
	return source;
}

void SourceController::removeSource(Source* source)
{
	if (!source) {
		emit errorOccurred({
			"Cannot remove source: source is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});
		qWarning() << "Cannot remove source: source is null";
		return;
	};

	// Get source id before deleting
	QUuid sourceId = boostUuidToQUuid(source->uuid());

	// Remove source from the list
	mSources.removeAll(source);

	emit sourceRemoved(sourceId); // TODO: Emit the source's ID instead of the source itself
}

Source* SourceController::getSource(QByteArray id) const
{
	for (Source* source : mSources) {
		QUuid uuid = boostUuidToQUuid(source->uuid());
		if (source && uuid.toRfc4122() == id) {
			return source;
		}
	}

	return nullptr;
}

void SourceController::clearSources()
{
	for (Source* source : mSources) {
		if (source) {
			QUuid sourceId = boostUuidToQUuid(source->uuid());
			source->deleteLater();
			emit sourceRemoved(sourceId);
		}
	}

	mSources.clear();
	mState = CLOSED;
	emit stateChanged(CLOSED);
}
