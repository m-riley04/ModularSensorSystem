#include "controllers/backendcontrollerbase.h"

BackendControllerBase::BackendControllerBase(QString controllerName, QObject *parent)
	: QObject(parent), mName(controllerName)
{}

BackendControllerBase::~BackendControllerBase()
{}

