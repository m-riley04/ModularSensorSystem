#include "videodevice.h"

VideoDevice::VideoDevice(RecordingSession* recordingSession, QObject* parent)
	: Device(recordingSession, parent)
{
	this->mName = "VideoDevice";
    this->mDeviceType = Device::Type::CAMERA;
    this->pDevicePreview = new VideoPreview(&mSession, this);
    open(); // TODO: Change this method to more represent it's purpose
}

VideoDevice::VideoDevice(QCameraDevice qVideoDevice, RecordingSession* recordingSession, QObject* parent)
	: Device(recordingSession, parent), mCamera(qVideoDevice)
{
    this->mName = qVideoDevice.description();
    this->mDeviceType = Device::Type::CAMERA;
	this->pDevicePreview = new VideoPreview(&mSession, this);
    open(); // TODO: Change this method to more represent it's purpose
}

VideoDevice::~VideoDevice()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void VideoDevice::open() {
	if (!mCamera.isAvailable()) {
		QMessageBox::warning(nullptr, "Camera Error", "No camera available.");
		return;
	}

    // Set default output location
	QDir _(QDir::currentPath()); 
	if (!_.exists("output")) {
		_.mkdir("output");
	}

    // Init preview
    auto pVideoPreview = new VideoPreview(&mSession, this);
	pDevicePreview = pVideoPreview;

    // Init buffer
	pVideoBuffer = std::make_unique<VideoClipBuffer>(2, this);

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");
    
    // Initialize capture session
    mSession.setCamera(&mCamera);
	mSession.setRecorder(&mRecorder);

    emit previewAvailable(this, pDevicePreview);
}

void VideoDevice::start() {
    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void VideoDevice::stop() {
    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

void VideoDevice::close()
{
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