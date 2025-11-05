#include "controllers/sourcecontroller.h"
#include <utils/boost_qt_conversions.h>

SourceController::SourceController(PluginController* pluginController, QObject *parent)
	: BackendControllerBase("SourceController", parent), pPluginController(pluginController)
{}

SourceController::~SourceController()
{}

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
}
