#pragma once

#include <string>

struct AngleBounds {
	double min = 0.0;
	double max = 0.0;
};

struct PoseBounds {
	AngleBounds yaw;
	AngleBounds pitch;
	AngleBounds roll;
};

// TODO: separate bounds/other info into its own struct?

struct Pose { // TODO: think about if this is the best way to do this
	double yaw, pitch, roll;
	PoseBounds bounds;
};

class IPanTiltMount
{
public:
	virtual ~IPanTiltMount() = default;

	/**
	 * @brief A combination of all pan-tilt pose info.
	 * @return
	 */
	virtual Pose pose() const = 0;

	/**
	 * @brief Refreshes the mount's info from the hardware.
	 * @return True if the refresh was successful, false otherwise.
	 */
	virtual bool refreshInfo() = 0;

	/**
	 * @brief Moves the mount to the specified pan and tilt angles.
	 * @param panAngle The target pan angle in degrees.
	 * @param tiltAngle The target tilt angle in degrees.
	 * @return True if the command was successful, false otherwise.
	 */
	virtual bool moveTo(double panAngle, double tiltAngle) = 0;

	/**
	 * @brief Recenters the mount to its default position.
	 * @return True if the command was successful, false otherwise.
	 */
	virtual bool recenter() = 0;
	
};