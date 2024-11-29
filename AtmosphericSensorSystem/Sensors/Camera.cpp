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
    stop();
}

void Camera::initialize() {
    // Default the video device
	if (checkCameraAvailability()) {
		setVideoDevice(0);
	}
}

void Camera::start() {
    // Check if camera is already running
    if (_running) return; // TODO: Do more logging here

    // Check if the camera is available
    if (!camera.isAvailable()) return; // TODO: Do more logging here

    // Open the camera
    camera.start();

    // Update state
    _running = true;

    // Initialize the timer

    connect(frameTimer, &QTimer::timeout, this, &Camera::captureFrame);
    
    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if not running
	if (!_running) return;

    // Update state
    _running = false;
    
    // Emit signal
    emit stopped();
}

QVariant Camera::read() {
    cv::Mat frame;
    if (camera.read(frame)) {
        return QVariant::fromValue(frame);
    }
    else {
        qWarning() << "Failed to read frame from camera.";
        return QVariant();
    }
}

bool Camera::checkCameraAvailability() {
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void Camera::captureFrame() {
    QVariant data = read();
    if (!data.isNull()) {
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        emit dataReady(data, timestamp);
    }
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