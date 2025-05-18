#include "videodevice.h"

VideoDevice::VideoDevice(QCameraDevice qVideoDevice, RecordingSession* recordingSession, QObject* parent)
	: Device(recordingSession, parent), mCamera(qVideoDevice)
{
    mId = qVideoDevice.id();
    mName = qVideoDevice.description();
    mDeviceType = Device::Type::CAMERA;
    pDevicePreview = new VideoPreview(&mSession, this); // TODO: Make this a unique_ptr?
    // TODO/CONSIDER: add initializing back for certain stuff?
}

VideoDevice::~VideoDevice()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void VideoDevice::open() {
	if (mState == OPENED || mState == STARTED) return; // Already opened or started

    // TODO: Move checks up/somewhere else?
	if (!mCamera.isAvailable()) {
		QMessageBox::warning(nullptr, "Camera Error", "No camera available.");
		return;
	}

    if (mCamera.isActive()) {
        QMessageBox::warning(nullptr, "Camera Error", "Camera is already active.");
        return;
    }

    // Initialize capture session
    mSession.setCamera(&mCamera);
    mSession.setRecorder(&mRecorder);

    // Init preview
	if (!pDevicePreview) pDevicePreview = new VideoPreview(&mSession, this);

    // Init buffer
	pVideoBuffer = std::make_unique<VideoClipBuffer>(2, this);

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");

    emit previewAvailable(this, pDevicePreview);
}

void VideoDevice::start() {
    if (mState == STARTED) return;

    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void VideoDevice::stop() {
    if (mState == STOPPED) return;

    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

void VideoDevice::close()
{
    if (mState == CLOSED || mState == STOPPED) return;
	// TODO: Implement close
}

void VideoDevice::onNewFrame(const QVideoFrame& frame) {
	// Get the timestamp
	ClipBufferBase::time timestamp = 0; // TODO: Get the timestamp from the frame
    
    // Add the frame to the buffer
	pVideoBuffer->push(frame, timestamp);
}

void VideoDevice::setMediaDirectory(QUrl directory)
{
	mRecorder.setOutputLocation(directory);
	emit mediaDirectoryChanged(directory);
}

bool VideoDevice::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

void VideoDevice::restart() {
    stop();
    start();
}