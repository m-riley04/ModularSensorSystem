#include "controllers/mountcontroller.h"

MountController::MountController(QObject *parent)
	: BackendControllerBase("MountController", parent)
{}

MountController::~MountController()
{}

const Mount* MountController::byId(const QUuid& id) const
{
	return mMountsById.value(id, nullptr);
}

QUuid MountController::idFor(const Mount* mount) const
{
	return mIdByMount.value(mount, QUuid());
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
	mIdByMount[mount] = uid;

	emit mountAdded(mount);
	return mount;
}

void MountController::removeMount(Mount* mount)
{
	if (!mount) {
		qWarning() << "Cannot remove source: source is null";
		return;
	};

	// Remove from lists/maps
	mMounts.removeAll(mount);
	QUuid uid = mIdByMount.take(mount);
	if (!uid.isNull()) {
		mMountsById.remove(uid);
	}

	emit mountRemoved(mount);
}

void MountController::removeMount(const QUuid& id)
{
	auto mount = mMountsById.value(id, nullptr);
	if (!mount) {
		qWarning() << "Cannot remove mount: mount not found";
		return;
	}

	mMounts.removeAll(mount);
	mMountsById.remove(id);
	mIdByMount.remove(mount);

	emit mountRemoved(mount);
}