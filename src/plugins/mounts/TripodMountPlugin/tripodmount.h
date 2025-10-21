#pragma once

#include "features/mounts/mount.h"

class TripodMount : public Mount
{

public:
	TripodMount();
	TripodMount(std::string id, std::string name);
	~TripodMount() = default;

	Kind kind() const override { return Kind::STATIC; };
	bool isMovable() const override { return false; }

	bool isPosable() const override { return false; }
	std::optional<Pose> pose() const override { return std::nullopt; }
	bool setPose(const Pose& newPose) override { return false; }
};

