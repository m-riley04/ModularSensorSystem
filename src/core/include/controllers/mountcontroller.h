#pragma once

#include "controllers/backendcontrollerbase.h"
#include <features/mounts/mount.h>
#include <QHash>
#include <QUuid>
#include <QDebug>
#include <sdk/plugins/imountplugin.h>

class MountController : public BackendControllerBase
{
	Q_OBJECT

public: 
	MountController(QObject *parent);
	~MountController();

	QList<Mount*> mounts() const { return mMounts; }

	const Mount* byId(const QUuid& id) const;

public slots:
	Mount* addMount(IMountPlugin* plugin, MountInfo mount);
	void removeMount(Mount* mount);
	void removeMount(const QUuid& id);

private:
	QList<Mount*> mMounts;
	QHash<QUuid, Mount*> mMountsById;

signals:
	void mountAdded(Mount*);
	void mountRemoved(Mount*);
};

