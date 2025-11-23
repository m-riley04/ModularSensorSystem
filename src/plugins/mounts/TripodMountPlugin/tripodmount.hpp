#pragma once

#include <qobject.h>
#include "features/mounts/mount.hpp"

class TripodMount : public Mount
{
	Q_OBJECT

public:
	TripodMount(ElementInfo& element, QObject* parent);
	~TripodMount() = default;

	bool isDynamic() const override { return false; }
	bool isPosable() const override { return true; }
	bool isAutomaticallyPosed() const override { return false; }

	std::optional<Pose> pose() const override { return std::nullopt; }
	bool setPose(const Pose& newPose) override { return false; }
};

