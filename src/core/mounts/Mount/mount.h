#pragma once

#include <QObject>

/**
 * @brief Mounts represent the physical "mount" that physical sources sit on. They provide data
 */
class Mount : public QObject
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

	struct MountPose { // TODO: think about if this is the best way to do this
		double x, y, z, yaw, pitch, roll;
	};

public:
	Mount(QByteArray id, QString name, Kind kind, QObject* parent);
	Mount(QObject *parent);
	~Mount();

	QByteArray id() const { return mId; }
	QString name() const { return mName; }
	Kind kind() const { return mKind; }
	MountPose pose() const { return mPose; }
	virtual bool isMovable() const { return false; }

protected:
	QByteArray mId;
	QString mName = "New Mount";
	Kind mKind = Kind::STATIC;
	MountPose mPose = MountPose();
};

