#pragma once

#include <string>
#include <optional>

/**
 * @brief Mounts represent the physical "mount" that physical sources sit on. They provide data
 */
class Mount
{

public:
	enum Kind {
		STATIC,
		PAN_TILT,
		GIMBAL,
		LINEAR_STAGE,
		MOBILE_BASE
	};

	struct Pose { // TODO: think about if this is the best way to do this
		double x, y, z, yaw, pitch, roll;
	};

public:
	Mount();
	virtual ~Mount() = default;

	std::string id() const { return mId; }
	std::string name() const { return mName; }
	virtual Kind kind() const = 0;
	virtual bool isMovable() const = 0;

	virtual bool isPosable() const = 0;
	virtual std::optional<Pose> pose() const = 0;
	virtual bool setPose(const Pose& newPose) = 0;

protected:
	Mount(std::string id, std::string name) : mId(id), mName(name) {}

	std::string mId;
	std::string mName = "New Mount";
};

