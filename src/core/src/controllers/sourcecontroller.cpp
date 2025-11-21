#include "controllers/sourcecontroller.hpp"

SourceController::SourceController(PluginController* pluginController, QObject *parent)
	: BackendControllerBase("SourceController", parent), pPluginController(pluginController)
{}

SourceController::~SourceController()
{}

QList<IRecordableSource*> SourceController::recordableSources() const
{
	QList<IRecordableSource*> recordableSourcesList;
	for (Source* source : mSources) {
		if (!source) continue;
		if (!source->asRecordable()) continue;
		recordableSourcesList.append(source->asRecordable());
	}
	return recordableSourcesList;
}

Source* SourceController::byId(const QUuid & id) const
{
	return mSourcesById.value(id);
}

Source* SourceController::addSource(ISourcePlugin* plugin, SourceInfo info) {
	if (!plugin) {
		qWarning() << "Plugin for source is null";
		return nullptr;
	}
	
	auto source = plugin->createSource(info.id, this);

	mSources.append(source);
	QUuid uid = boostUuidToQUuid(source->uuid());
	mSourcesById[uid] = source;

	emit sourceAdded(source);
	return source;
}

void SourceController::removeSource(Source* source)
{
	if (!source) {
		qWarning() << "Cannot remove source: source is null";
		return;
	};

	// Get source id before deleting
	QUuid sourceId = boostUuidToQUuid(source->uuid());

	// Remove source from the list
	mSources.removeAll(source);
	mSourcesById.remove(sourceId);
	source->deleteLater();

	emit sourceRemoved(sourceId); // TODO: Emit the source's ID instead of the source itself
}

void SourceController::removeSource(const QUuid& uuid)
{
	Source* source = byId(uuid);
	if (!source) {
		qWarning() << "Cannot remove source: source with given UUID not found";
		return;
	}
	removeSource(source);
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
}
