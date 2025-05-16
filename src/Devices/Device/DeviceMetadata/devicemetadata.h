#pragma once

#include <QObject>
#include <QVector3d>
#include <qquaternion.h>

class DeviceMetadata  : public QObject
{
	Q_OBJECT

public:
	DeviceMetadata(QObject *parent);
	~DeviceMetadata();

private:
	QVector3D mPosition;
	QQuaternion mOrientation;
	QVector<float> mDistortionCoefficients;
	QString mNotes;
};
