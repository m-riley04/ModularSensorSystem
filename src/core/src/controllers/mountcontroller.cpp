#include "controllers/mountcontroller.hpp"
#include <controllers/loggingcontroller.hpp>

MountController::MountController(QObject *parent)
	: QObject(parent)
{}

MountController::~MountController()
{
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

	auto* mount = plugin->createMount(info.id, this);
	if (!mount) return nullptr;

	// Enforce ownership
	if (mount->parent() != this) {
		mount->setParent(this);
	}

	QUuid uid = boostUuidToQUuid(mount->uuid());

	mMounts.append(QPointer<Mount>(mount));
	mMountsById[uid] = QPointer<Mount>(mount);

	// If the mount is destroyed elsewhere, purge stale entries
	connect(mount, &QObject::destroyed, this, [this, uid]() {
		mMountsById.remove(uid);
		// Remove null guarded pointers from the list
		for (int i = mMounts.size() - 1; i >= 0; --i)
			if (mMounts[i].isNull())
				mMounts.removeAt(i);
		});

	emit mountAdded(mount);
	return mount;
}

void MountController::removeMount(Mount* mount)
{
	if (!mount) {
		LoggingController::warning("Cannot remove mount: mount is null");
		return;
	}

	QUuid uid = boostUuidToQUuid(mount->uuid());
	
	// Disconnect all signals from mount
	disconnect(mount, nullptr, this, nullptr);

	// Remove from lists/maps
	mMounts.removeAll(mount);
	mMountsById.remove(uid);

	emit mountRemoved(uid);

	// Schedule the mount for deletion (safe, deferred deletion)
	mount->deleteLater();
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

void MountController::clearMounts()
{
	QList<QPointer<Mount>> mountsCopy = mMounts; // Copy to avoid modification during iteration
	for (auto& mountPtr : mountsCopy) {
		removeMount(mountPtr);
	}
	mMounts.clear();
	mMountsById.clear();
}
