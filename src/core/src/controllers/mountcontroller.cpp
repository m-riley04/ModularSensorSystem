#include "controllers/mountcontroller.h"

MountController::MountController(QObject *parent)
	: BackendControllerBase("MountController", parent)
{}

MountController::~MountController()
{}

const Mount* MountController::byId(const QUuid& id) const
{
	// TODO: Implement the logic to find a Mount by its ID
	return nullptr;
}

Mount* MountController::addMount(IMountPlugin* plugin, MountInfo info)
{
	if (!plugin) {
		/*emit errorOccurred({
			"Cannot add source: plugin is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});
		qWarning() << "Plugin for source is null";*/
		return nullptr;
	}

	auto mount = plugin->createMount(info.id, this);

	mMounts.append(mount);
	mMountsById[QUuid::fromString(mount->id())] = mount;

	// Connect errors
	//connect(mount, &Mount::errorOccurred, this, &MountController::errorOccurred);

	emit mountAdded(mount);
	return mount;
}

void MountController::removeMount(Mount* mount)
{
	if (!mount) {
		/*emit errorOccurred({
			"Cannot remove source: source is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});*/
		qWarning() << "Cannot remove source: source is null";
		return;
	};

	// Get source id before deleting
	std::string mountId = mount->id();

	// Remove source from the list
	mMounts.removeAll(mount);

	emit mountRemoved(mount); // TODO: Emit the mount's ID instead of the mount itself
}

void MountController::removeMount(const QUuid& id)
{
	auto mount = mMountsById.value(id, nullptr);
	if (!mount) {
		/*emit errorOccurred({
			"Cannot remove mount: mount not found",
			nullptr,
			ErrorSeverity::CRITICAL
		});*/
		qWarning() << "Cannot remove mount: mount not found";
		return;
	}

	// Remove mount from the list
	mMounts.removeAll(mount);
	mMountsById.remove(id);

	emit mountRemoved(mount);
}