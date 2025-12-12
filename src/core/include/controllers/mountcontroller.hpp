#pragma once

#include "features/mounts/mount.hpp"
#include <QHash>
#include <QUuid>
#include "sdk/plugins/imountplugin.hpp"
#include "utils/boost_qt_conversions.hpp"
#include "core_export.hpp"

class MSS_CORE_API MountController : public QObject
{
	Q_OBJECT

public: 
	MountController(QObject *parent);
	~MountController();

	const QList<Mount*>& mounts() const { return mMounts; }

	const Mount* byId(const QUuid& id) const;

public slots:
	Mount* addMount(IMountPlugin* plugin, ElementInfo mount);
	void removeMount(Mount* mount);
	void removeMount(const QUuid& id);

private:
	QList<Mount*> mMounts;
	QHash<QUuid, Mount*> mMountsById;

signals:
	void mountAdded(Mount*);
	void mountRemoved(QUuid);
};

