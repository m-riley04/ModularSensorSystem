#include "controllers/mountcontroller.hpp"
#include <controllers/loggingcontroller.hpp>

MountController::MountController(QObject *parent)
	: QObject(parent)
{}

MountController::~MountController()
{
	// Clean up mounts
	/*for (Mount* mount : mMounts) {
		if (!mount) continue;
		disconnect(mount, nullptr, this, nullptr);
		mount->deleteLater();
	}*/
	mMounts.clear();
	mMountsById.clear();

}

Mount* MountController::byId(const QUuid& id) const
{
	return mMountsById.value(id, nullptr);
}

Mount* MountController::addMount(IMountPlugin* plugin, ElementInfo info)
{
	if (!plugin) {
		return nullptr;
	}

	auto mount = plugin->createMount(info.id, this);
	if (!mount) return nullptr;

	mMounts.append(mount);
	QUuid uid = boostUuidToQUuid(mount->uuid());
	mMountsById[uid] = mount;

	emit mountAdded(mount);
	return mount;
}

void MountController::removeMount(Mount* mount)
{
	if (!mount) {
		LoggingController::warning("Cannot remove source: source is null");
		return;
	};

	QUuid uid = boostUuidToQUuid(mount->uuid());
	
	// Disconnect all signals from mount
	disconnect(mount, nullptr, this, nullptr);

	// Remove from lists/maps
	mMounts.removeAll(mount);
	mMounts.squeeze();
	mMountsById.remove(uid);

	emit mountRemoved(uid);
}

void MountController::removeMount(const QUuid& id)
{
	auto mount = mMountsById.value(id, nullptr);
	if (!mount) {
		LoggingController::warning("Cannot remove mount: mount not found");
		return;
	}

	// Use existing method (slightly slower, since it gets the ID again, but cleaner)
	removeMount(mount);
}