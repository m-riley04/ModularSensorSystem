#pragma once

#include <qobject.h>
#include "features/mounts/mount.hpp"

class TripodMount : public Mount
{
	Q_OBJECT

public:
	TripodMount(const ElementInfo& element, QObject* parent);
	~TripodMount() = default;

	MountError error() const override final { return MountError{}; }
};

