#pragma once

#include <QObject>
#include <QVector3d>
#include <qquaternion.h>

class SourceMetadata  : public QObject
{
	Q_OBJECT

public:
	SourceMetadata(QObject *parent): QObject(parent) {}
	~SourceMetadata() = default;

private:
	QVector3D mPosition;
	QQuaternion mOrientation;
	QVector<float> mDistortionCoefficients;
	QString mNotes;
};
