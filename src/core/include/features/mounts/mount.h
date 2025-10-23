#pragma once

#include <string>
#include <optional>
#include "features/ielement.h"

/**
 * @brief Mounts represent the physical "mount" that physical sources sit on. They provide data
 */
class Mount : public IElement
{
	Q_OBJECT

public:
	enum Kind {
		STATIC,
		PAN_TILT,
		GIMBAL,
		LINEAR_STAGE,
		MOBILE_BASE
	};

	struct Pose { // TODO: think about if this is the best way to do this
		double x, y, z;
		double yaw, pitch, roll;
	};

public:
	Mount(QObject* parent) : IElement(parent) {}
	Mount(std::string id, std::string name, QObject* parent) : IElement(parent), m_id(id), m_name(name) {}
	virtual ~Mount() = default;

	std::string id() const override { return m_id; }
	std::string name() const override { return m_name; }
	void setName(const std::string& newName) override { m_name = newName; }
	virtual Kind kind() const = 0;
	virtual bool isMovable() const = 0;

	virtual bool isPosable() const = 0;
	virtual std::optional<Pose> pose() const = 0;
	virtual bool setPose(const Pose& newPose) = 0;

protected:
	std::string m_id = "default_mount";
	std::string m_name = "New Mount";
};

