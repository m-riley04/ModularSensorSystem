#include "mount.h"

Mount::Mount(QByteArray id, QString name, Kind kind, QObject* parent)
	: QObject(parent), mId(id), mName(name), mKind(kind)
{
}

Mount::Mount(QObject *parent)
	: QObject(parent), mKind(Kind::STATIC), mPose(MountPose())
{}

Mount::~Mount()
{}

