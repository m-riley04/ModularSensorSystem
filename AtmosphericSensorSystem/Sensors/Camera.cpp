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
    
    // Initialize the timer
	frameTimer = new QTimer();
    connect(frameTimer, &QTimer::timeout, this, &Camera::captureFrame);
    frameTimer->start(1000 / _fps);
    frameTimer->setInterval(static_cast<int>(1000 / _fps));

    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if camera is good to go/is active
    if (!camera.isAvailable() || !camera.isActive()) return; // TODO: Do more logging here
    
    // Stop timer
    frameTimer->stop();

    // Stop camera
    camera.stop();

    // Emit signal
    emit stopped();
}

QVariant Camera::read() {
    QVideoFrame frame = sink.videoFrame();

    return QVariant::fromValue(frame);
}

bool Camera::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

void Camera::captureFrame() {
    QVariant data = read();
    if (!data.isNull()) {
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        emit dataReady(data, timestamp);
    }
    else {
        qDebug() << "ERROR: Failed to read and capture frame from camera.";
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