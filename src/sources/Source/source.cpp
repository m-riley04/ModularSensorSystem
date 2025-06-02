#include "source.h"

Source::Source(QByteArray hardwareId, QObject* parent)
{
	mId = hardwareId;
}

Source::Source(QObject *parent)
	: QObject(parent)
{}

Source::~Source()
{}

