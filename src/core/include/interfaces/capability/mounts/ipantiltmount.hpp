#pragma once

class IPanTiltMount
{
public:
	virtual ~IPanTiltMount() = default;

	/**
	 * @brief Moves the mount to the specified pan and tilt angles.
	 * @param panAngle The target pan angle in degrees.
	 * @param tiltAngle The target tilt angle in degrees.
	 * @return True if the command was successful, false otherwise.
	 */
	virtual bool moveTo(double panAngle, double tiltAngle) = 0;

	/**
	 * @brief Gets the current pan angle of the mount.
	 * @return The current pan angle in degrees.
	 */
	virtual double panAngle() const = 0;

	/**
	 * @brief The INCLUSIVE minimum angle the mount can pan to.
	 * @return 
	 */
	virtual double panMinAngle() const = 0;

	/**
	 * @brief The INCLUSIVE maximum angle the mount can pan to.
	 * @return 
	 */
	virtual double panMaxAngle() const = 0;

	/**
	 * @brief Gets the current tilt angle of the mount.
	 * @return The current tilt angle in degrees.
	 */
	virtual double tiltAngle() const = 0;

	/**
	 * @brief The INCLUSIVE minimum angle the mount can tilt to.
	 * @return 
	 */
	virtual double tiltMinAngle() const = 0;

	/**
	 * @brief The INCLUSIVE maximum angle the mount can tilt to.
	 * @return 
	 */
	virtual double tiltMaxAngle() const = 0;

	/**
	 * @brief Recenters the mount to its default position.
	 * @return True if the command was successful, false otherwise.
	 */
	virtual bool recenter() = 0;
};