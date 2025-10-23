#pragma once

#include <qobject.h>
#include "features/mounts/mount.h"

class TripodMount : public Mount
{
	Q_OBJECT

public:
	TripodMount(QObject* parent);
	TripodMount(std::string id, std::string name, QObject* parent);
	~TripodMount() = default;

	Kind kind() const override { return Kind::STATIC; };
	bool isMovable() const override { return false; }

	bool isPosable() const override { return false; }
	std::optional<Pose> pose() const override { return std::nullopt; }
	bool setPose(const Pose& newPose) override { return false; }
};

