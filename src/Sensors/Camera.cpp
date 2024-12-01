#include "Camera.h"
#include <QtConcurrent>
#include <chrono>

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
    
    // Initialize capture session
    mSession.setVideoSink(&mSink);
    mSession.setCamera(&mCamera);
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

QVideoWidget* Camera::output()
{
    return pOutput;
}

void Camera::setOutput(QVideoWidget* widget)
{
    mSession.setVideoOutput(widget);
    pOutput = widget;
}

QCameraDevice* Camera::device()
{
    return pDevice;
}

void Camera::setDevice(QCameraDevice device)
{
    mCamera.setCameraDevice(device);
    pDevice = &device;
    emit deviceChanged();
}

bool Camera::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

void Camera::restart() {
    stop();
    start();
}

void Camera::setActive(bool active)
{
    mCamera.setActive(active);
}

void Camera::setAutoExposureTime(bool enable)
{
    mCamera.setAutoExposureTime();
}

void Camera::setAutoIsoSensitivity(bool enable)
{
    mCamera.setAutoIsoSensitivity();
}

void Camera::setColorTemperature(int temp)
{
    mCamera.setColorTemperature(temp);
}

void Camera::setExposureCompensation(float ev)
{
    mCamera.setExposureCompensation(ev);
}

void Camera::setExposureMode(QCamera::ExposureMode mode)
{
    mCamera.setExposureMode(mode);
}

void Camera::setFlashMode(QCamera::FlashMode mode)
{
    mCamera.setFlashMode(mode);
}

void Camera::setManualExposureTime(float seconds)
{
    mCamera.setManualExposureTime(seconds);
}

void Camera::setManualIsoSensitivity(int iso)
{
    mCamera.setManualIsoSensitivity(iso);
}

void Camera::setTorchMode(QCamera::TorchMode mode)
{
    mCamera.setTorchMode(mode);
}

void Camera::setWhiteBalanceMode(QCamera::WhiteBalanceMode mode)
{
    mCamera.setWhiteBalanceMode(mode);
}

void Camera::zoomTo(float factor, float rate)
{

}
