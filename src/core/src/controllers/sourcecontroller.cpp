#include "controllers/sourcecontroller.hpp"
#include <controllers/loggingcontroller.hpp>

SourceController::SourceController(QObject *parent)
	: QObject(parent)
{}

SourceController::~SourceController()
{
	mSources.clear();
	mSourcesById.clear();
}

const QList<IPreviewable*> SourceController::previewableSources() const
{
	QList<IPreviewable*> previewableSourcesList;
	for (Source* source : mSources) {
		if (!source) continue;
		if (auto s = source->asPreviewable()) previewableSourcesList.append(s);
	}
	return previewableSourcesList;
}

const QList<IRecordable*> SourceController::recordableSources() const
{
	QList<IRecordable*> recordableSourcesList;
	for (Source* source : mSources) {
		if (!source) continue;
		if (auto s = source->asRecordable()) recordableSourcesList.append(s);
	}
	return recordableSourcesList;
}

Source* SourceController::byId(const QUuid & id) const
{
	return mSourcesById.value(id);
}


Source* SourceController::addSource(ISourcePlugin* plugin, SourceInfo info) {
	if (!plugin) {
		LoggingController::warning("Cannott add source; plugin is null");
		return nullptr;
	}

	auto source = plugin->createSource(info.elementInfo.id, this);
	if (!source) {
		LoggingController::warning("Could not create source for element " + QString::fromStdString(info.elementInfo.id));
		return nullptr;
	}

	mSources.append(source);
	QUuid uid = boostUuidToQUuid(source->uuid());
	mSourcesById[uid] = source;

	emit sourceAdded(source);
	return source;
}

void SourceController::removeSource(Source* source)
{
	if (!source) {
		LoggingController::warning("Cannot remove source: source is null");
		return;
	};

	// Get source id before deleting
	QUuid sourceId = boostUuidToQUuid(source->uuid());

	// Remove source from the list
	mSources.removeAll(source);
	mSourcesById.remove(sourceId);

	emit sourceRemoved(sourceId); // TODO: Emit the source's ID instead of the source itself

	// Schedule the source for deletion (safe, deferred deletion)
	source->deleteLater();
}

void SourceController::removeSource(const QUuid& uuid)
{
	Source* source = byId(uuid);
	if (!source) {
		LoggingController::warning("Cannot remove source: source with given UUID not found");
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
	// Emit signals first, without modifying the list during ranged-for
	QList<Source*> sourcesCopy = mSources;
	for (Source* source : sourcesCopy) {
		if (!source) continue;
		removeSource(source);
	}
	mSources.clear();
	mSourcesById.clear();
}
