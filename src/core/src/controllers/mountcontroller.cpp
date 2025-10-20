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

void MountController::addMount(Mount* mount) {

}

void MountController::removeMount(Mount* mount) {

}

void MountController::removeMount(const QUuid& id) {

}