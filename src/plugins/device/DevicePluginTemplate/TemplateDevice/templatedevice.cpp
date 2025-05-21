#include "templatedevice.h"

TemplateDevice::TemplateDevice(QObject* parent)
	: Device(parent)
{
    mId = QByteArray(); // TODO: Initialize UNIQUE hardware ID
	mName = "Template Device"; // TODO: Initialize device name
    mDeviceType = Device::Type::OTHER; // TODO: Initialize device type
    pPreview.reset(new TemplatePreview(this)); // TODO: Make this a unique_ptr?
}

TemplateDevice::~TemplateDevice()
{}

void TemplateDevice::open() {
	if (mState == OPENED || mState == STARTED) return; // Already opened or started

    // Init preview
	if (!pPreview) pPreview.reset(new TemplatePreview(this));

    // Init clipping buffer
    if (!pClipBuffer) pClipBuffer.reset(new TemplateClipBuffer(2, this));

    emit previewAvailable(this, pPreview.get());
}

void TemplateDevice::start() {
    if (mState == STARTED) return;

    emit started();
}

void TemplateDevice::stop() {
    if (mState == STOPPED) return;

    emit stopped();
}

void TemplateDevice::close()
{
    if (mState == CLOSED || mState == STOPPED) return;
}

void TemplateDevice::restart() {
    stop();
    start();
}

void TemplateDevice::beginRecording(RecordingSession* s)
{
	// Update recording session
	pRecordingSession = s;

	// Start recording
	
}

void TemplateDevice::endRecording()
{
	// Stop recording
}
