#include "source.h"

Source::Source(QByteArray hardwareId, QObject* parent)
{
	mId = QUuid::fromBytes(hardwareId); // Must be big endian to be reversible. TODO: make sure this works on all platforms
}

Source::Source(QObject *parent)
	: QObject(parent)
{}

Source::~Source()
{}

