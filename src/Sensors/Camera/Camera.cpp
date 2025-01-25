#include "Camera.h"

Camera::Camera(QObject *parent)
	: Sensor(parent)
{
    // Default the camera
    initialize();
}

Camera::~Camera()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void Camera::initialize() {
    // Default the video device
	if (checkCameraAvailability()) {
		setDevice(QCameraDevice());
	}

    // Set default output location
	QDir _(QDir::currentPath()); 
	if (!_.exists("output")) {
		_.mkdir("output");
	}

    mRecorder.setOutputLocation(QDir::currentPath() + "/output");
    
    // Initialize capture session
    mSession.setCamera(&mCamera);
    mSession.setVideoSink(&mSink);
	mSession.setRecorder(&mRecorder);
    mSession.setAudioInput(&mAudioInput);

    // Initialize connections
    connect(&mAudioInput, &QAudioInput::deviceChanged, [this]() {
        emit audioDeviceChanged(mAudioInput.device());
        });
    connect(&mCamera, &QCamera::cameraDeviceChanged, [this]() {
        emit cameraDeviceChanged(mCamera.cameraDevice());
        });
}

void Camera::start() {
    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

QVariant Camera::read() {
    QVideoFrame frame = mSink.videoFrame();

    return QVariant::fromValue(frame);
}

void Camera::setOutput(QVideoWidget* widget)
{
    mSession.setVideoOutput(widget);
}

void Camera::setDevice(QCameraDevice device)
{
    mCamera.setCameraDevice(device);
    emit cameraDeviceChanged(device);
}

void Camera::setMediaDirectory(QUrl directory)
{
	mRecorder.setOutputLocation(directory);
	emit mediaDirectoryChanged(directory);
}

bool Camera::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

void Camera::restart() {
    stop();
    start();
}