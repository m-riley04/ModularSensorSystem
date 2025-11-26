#pragma once

#include <string>
#include <optional>
#include "features/element.hpp"

/**
 * @brief Mounts represent the physical "mount" that physical sources sit on. They provide data
 */
class Mount : public Element
{
	Q_OBJECT

public:
	struct Pose { // TODO: think about if this is the best way to do this
		double x, y, z;
		double yaw, pitch, roll;
	};

public:
	Mount(const ElementInfo& element, QObject* parent) : Element(element, parent) {}
	virtual ~Mount() = default;

	/**
	 * @brief Indicates if the mount is dynamic (i.e., can change position over time).
	 * Most times, you likely want static mounts for the most accurate results.
	 * @return True if the mount is dynamic, false otherwise.
	 */
	virtual bool isDynamic() const = 0;

	/**
	 * @brief Indicates if the mount is posable (i.e., can have its pose set during runtime).
	 * @return True if the mount is posable, false otherwise.
	 */
	virtual bool isPosable() const = 0;

	/**
	 * @brief Indicates if the mount is automatically posed by an external system (e.g., tracking system).
	 * @return True if the mount is automatically posed, false otherwise.
	 */
	virtual bool isAutomaticallyPosed() const = 0;

	/**
	 * @brief The current pose of the mount, if available.
	 * @return The current pose of the mount, or an empty optional if not available.
	 */
	virtual std::optional<Pose> pose() const = 0;
	virtual bool setPose(const Pose& newPose) = 0;

};

