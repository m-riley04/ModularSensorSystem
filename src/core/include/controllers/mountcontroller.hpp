#pragma once

#include "features/mounts/mount.hpp"
#include <QHash>
#include <QUuid>
#include "sdk/plugins/imountplugin.hpp"
#include "utils/boost_qt_conversions.hpp"
#include "core_export.hpp"
#include <QPointer>

class MSS_CORE_API MountController : public QObject
{
	Q_OBJECT

public: 
	MountController(QObject *parent);
	~MountController();

	const QList<QPointer<Mount>>& mounts() const { return mMounts; }

	Mount* byId(const QUuid& id) const;

public slots:
	Mount* addMount(IMountPlugin* plugin, ElementInfo mount);
	void removeMount(Mount* mount);
	void removeMount(const QUuid& id);
	void clearMounts();

private:
	QList<QPointer<Mount>> mMounts;
	QHash<QUuid, QPointer<Mount>> mMountsById;

signals:
	void mountAdded(Mount*);
	void mountRemoved(QUuid);
};

