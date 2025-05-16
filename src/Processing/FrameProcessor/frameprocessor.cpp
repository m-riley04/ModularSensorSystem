#include "frameprocessor.h"

FrameProcessor::FrameProcessor(QObject *parent)
	: QObject(parent)
{}

FrameProcessor::~FrameProcessor()
{}

void FrameProcessor::setEnabled(bool enabled) {
	if (enabled == mEnabled) return;
	mEnabled = enabled;
	emit enabledChanged(mEnabled);
}
void FrameProcessor::setName(const QString& name) {
	if (name == mName) return;
	mName = name;
	emit nameChanged(mName);
}
void FrameProcessor::setLastProcessTime(qint64 time) {
	if (time == mLastProcessTime) return;
	mLastProcessTime = time;
	emit lastProcessTimeChanged(mLastProcessTime);
}
