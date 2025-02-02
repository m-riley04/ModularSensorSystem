#include "Camera.h"

Camera::Camera(QObject *parent, VideoInputMethod method)
	: Sensor(parent), mVideoInputMethod(method)
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

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");
    
    // Initialize capture session
    if (mVideoInputMethod == VideoInputMethod::QCAMERA) mSession.setCamera(&mCamera);
    if (mVideoInputMethod == VideoInputMethod::QVIDEOFRAMEINPUT) mSession.setVideoFrameInput(&mFrameInput);
    mSession.setVideoSink(&mSink);
	mSession.setRecorder(&mRecorder);
    mSession.setAudioInput(&mAudioInput);

    // Initialize deep signal connections
    connect(&mAudioInput, &QAudioInput::deviceChanged, [this]() {
        emit audioDeviceChanged(mAudioInput.device());
        });
    connect(&mCamera, &QCamera::cameraDeviceChanged, [this]() {
        emit cameraDeviceChanged(mCamera.cameraDevice());
        });

	// Initialize generator signal connections
    connect(&mFrameInput, &QVideoFrameInput::readyToSendVideoFrame,
        &mGenerator, &OpenCVGenerator::nextFrame);
    connect(&mGenerator, &OpenCVGenerator::frameReady,
        &mFrameInput, [this](const QVideoFrame& frame) {
            bool successfullySent = mFrameInput.sendVideoFrame(frame);

            if (!successfullySent) {
                qDebug() << "Error: Could not send video frame";
            }
        });
}

void Camera::changeVideoInputMethod(VideoInputMethod method)
{
	if (mVideoInputMethod == method) return;
    mVideoInputMethod = method;

    switch (mVideoInputMethod) {
	case VideoInputMethod::QCAMERA:
        mSession.setVideoFrameInput(nullptr);
		mSession.setCamera(&mCamera);
        mGenerator.stop();
        mCamera.start();
		break;

	case VideoInputMethod::QVIDEOFRAMEINPUT:
        mSession.setCamera(nullptr);
        mCamera.stop();
        mGenerator.start(); /// TODO: Set this to correct input index
		mSession.setVideoFrameInput(&mFrameInput);
		break;
    }

	emit videoInputMethodChanged(mVideoInputMethod);
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

void Camera::setVideoOutput(QVideoWidget* widget)
{
	if (mSession.videoOutput() == widget) return;

    mSession.setVideoOutput(widget);
    emit videoOutputChanged(widget);
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