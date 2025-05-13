#include "cameradevice.h"

CameraDevice::CameraDevice(QObject *parent)
	: AbstractDevice(parent)
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
	pVideoBuffer = new VideoBuffer(2, DEFAULT_FRAME_RATE, this); // TODO: make sure the frame rate is updated whenever the format is

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
    connect(widget->videoSink(), &QVideoSink::videoFrameChanged, [this](QVideoFrame frame) {
        // Add the frame to the buffer
        pVideoBuffer->addFrame(frame);
        });
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