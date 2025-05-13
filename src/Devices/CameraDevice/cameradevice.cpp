#include "cameradevice.h"

CameraDevice::CameraDevice(QObject *parent)
	: Device(parent)
{
    // init camera
    open();
}

CameraDevice::~CameraDevice()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void CameraDevice::open() {
    // Default the video device
	if (checkCameraAvailability()) {
		setDevice(QCameraDevice());
	}

    // Set default output location
	QDir _(QDir::currentPath()); 
	if (!_.exists("output")) {
		_.mkdir("output");
	}

    // Init buffer
	pVideoBuffer = std::make_unique<VideoClipBuffer>(2, this);

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");
    
    // Initialize capture session
    mSession.setCamera(&mCamera);
	mSession.setRecorder(&mRecorder);
    mSession.setAudioInput(&mAudioInput);

    // Initialize deep signal connections
    connect(&mAudioInput, &QAudioInput::deviceChanged, [this]() {
        emit audioDeviceChanged(mAudioInput.device());
        });
    connect(&mCamera, &QCamera::cameraDeviceChanged, [this]() {
        emit cameraDeviceChanged(mCamera.cameraDevice());
        });
}

void CameraDevice::start() {
    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void CameraDevice::stop() {
    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

void CameraDevice::setVideoOutput(CustomSinkWidget* widget)
{
    if (widget == nullptr) return;

	if (mSession.videoSink() == widget->videoSink()) return;

    pSinkWidget = widget;

    mSession.setVideoOutput(widget->videoSink());
    emit videoOutputChanged(widget);

	// Connect the video sink to the buffer
    connect(widget->videoSink(), &QVideoSink::videoFrameChanged, onNewFrame);
}

void CameraDevice::onNewFrame(const QVideoFrame& frame) {
	// Get the timestamp
	ClipBufferBase::time timestamp = 0; // TODO: Get the timestamp from the frame
    
    // Add the frame to the buffer
	pVideoBuffer->push(frame, timestamp);
}

void CameraDevice::setDevice(QCameraDevice device)
{
    mCamera.setCameraDevice(device);
    emit cameraDeviceChanged(device);
}

void CameraDevice::setMediaDirectory(QUrl directory)
{
	mRecorder.setOutputLocation(directory);
	emit mediaDirectoryChanged(directory);
}

bool CameraDevice::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

void CameraDevice::restart() {
    stop();
    start();
}