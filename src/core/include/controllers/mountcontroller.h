#pragma once

#include "controllers/backendcontrollerbase.h"
#include <features/mounts/mount.h>
#include <QHash>
#include <QUuid>

class MountController : public BackendControllerBase
{
	Q_OBJECT

protected:
	QString mName = "MountController";

public: 
	MountController(QObject *parent);
	~MountController();

	QList<Mount*> mounts() const { return mMounts; }

	const Mount* byId(const QUuid& id) const;

private:
	QList<Mount*> mMounts;
	QHash<QUuid, Mount*> mMountsById;

signals:
	void mountAdded(Mount*);
	void mountRemoved(Mount*);
};

