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
    if (camera.isActive()) {
        stop();
    }
}

void Camera::setVideoDevice(QCameraDevice device)
{
    camera.setCameraDevice(device);
    emit deviceChanged();
}

void Camera::initialize() {
    // Default the video device
	if (checkCameraAvailability()) {
		setVideoDevice(0);
	}
    
    // Initialize capture session
    session.setVideoSink(&sink);
    session.setCamera(&camera);
}

void Camera::start() {
    // Check if the camera is available/is idle
    if (!camera.isAvailable() || camera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    camera.start();

    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if camera is good to go/is active
    if (!camera.isAvailable() || !camera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    camera.stop();

    // Emit signal
    emit stopped();
}

QVariant Camera::read() {
    QVideoFrame frame = sink.videoFrame();

    return QVariant::fromValue(frame);
}

void Camera::setVideoWidget(QVideoWidget* widget)
{
    session.setVideoOutput(widget);
}

void Camera::restart() {
    stop();
    start();
}

void Camera::setVideoDevice(int deviceIndex) {
    // Set new index
    camera.setCameraDevice(QCameraDevice()); // TODO: Actually use device index
	emit deviceChanged();
}

bool Camera::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}