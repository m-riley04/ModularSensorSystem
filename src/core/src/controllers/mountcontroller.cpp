#include "controllers/mountcontroller.h"

MountController::MountController(QObject *parent)
	: BackendControllerBase(mName, parent)
{}

MountController::~MountController()
{}

const Mount* MountController::byId(const QUuid& id) const
{
	// TODO: Implement the logic to find a Mount by its ID
	return nullptr;
}