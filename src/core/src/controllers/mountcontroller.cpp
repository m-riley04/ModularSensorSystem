#include "controllers/mountcontroller.h"
#include <utils/boost_qt_conversions.h>

MountController::MountController(QObject *parent)
	: BackendControllerBase("MountController", parent)
{}

MountController::~MountController()
{}

const Mount* MountController::byId(const QUuid& id) const
{
	return mMountsById.value(id, nullptr);
}

Mount* MountController::addMount(IMountPlugin* plugin, MountInfo info)
{
	if (!plugin) {
		return nullptr;
	}

	auto mount = plugin->createMount(info.id, this);
	if (!mount) return nullptr;

	mMounts.append(mount);
	// Assign a controller-managed UUID (do not reinterpret mount->id() as a QUuid)
	QUuid uid = QUuid::createUuid();
	mMountsById[uid] = mount;

	emit mountAdded(mount);
	return mount;
}

void MountController::removeMount(Mount* mount)
{
	if (!mount) {
		qWarning() << "Cannot remove source: source is null";
		return;
	};

	QUuid uid = boostUuidToQUuid(mount->uuid());
	
	// Remove from lists/maps
	mMounts.removeAll(mount);
	mMountsById.remove(uid);

	emit mountRemoved(uid);
}

void MountController::removeMount(const QUuid& id)
{
	auto mount = mMountsById.value(id, nullptr);
	if (!mount) {
		qWarning() << "Cannot remove mount: mount not found";
		return;
	}

	// Use existing method (slightly slower, since it gets the ID again, but cleaner)
	removeMount(mount);
}