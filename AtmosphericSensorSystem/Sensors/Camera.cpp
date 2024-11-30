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
    if (m_camera.isActive()) {
        stop();
    }
}

void Camera::setVideoDevice(QCameraDevice device)
{
    m_camera.setCameraDevice(device);
    emit deviceChanged();
}

void Camera::initialize() {
    // Default the video device
	if (checkCameraAvailability()) {
		setVideoDevice(0);
	}
    
    // Initialize capture session
    m_session.setVideoSink(&m_sink);
    m_session.setCamera(&m_camera);
}

void Camera::start() {
    // Check if the camera is available/is idle
    if (!m_camera.isAvailable() || m_camera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    m_camera.start();

    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if camera is good to go/is active
    if (!m_camera.isAvailable() || !m_camera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    m_camera.stop();

    // Emit signal
    emit stopped();
}

QVariant Camera::read() {
    QVideoFrame frame = m_sink.videoFrame();

    return QVariant::fromValue(frame);
}

void Camera::setVideoWidget(QVideoWidget* widget)
{
    m_session.setVideoOutput(widget);
}

void Camera::restart() {
    stop();
    start();
}

void Camera::setVideoDevice(int deviceIndex) {
    // Set new index
    m_camera.setCameraDevice(QCameraDevice()); // TODO: Actually use device index
	emit deviceChanged();
}

bool Camera::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}