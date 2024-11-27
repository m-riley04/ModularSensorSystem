#include "Camera.h"
#include <QtConcurrent>
#include <chrono>

Camera::Camera(QObject *parent)
	: Sensor(parent), frameTimer(new QTimer())
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

    // Set/calculate the FPS /// TODO: Initialize all the camera properties
    setFPS(_fps);
}

void Camera::start() {
    // Check if camera is already running
    if (_running) return;

    // Update state
    _running = true;

    // Open the camera
    camera.open(_videoDeviceIndex, cv::CAP_DSHOW);
    if (!isOpened()) {
        emit error("Failed to open camera.");
        return;
    }

    // Initialize the timer
    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &Camera::captureFrame);
    frameTimer->start(1000 / _fps);
    
    // Emit signal
    emit started();
}

void Camera::stop() {
    // Check if not running
	if (!_running) return;

    // Update state
    _running = false;

    // Stop the frame timer
    frameTimer->stop();

    // Release the camera
    camera.release();
    
    // Emit signal
    emit stopped();
}

bool Camera::isOpened() { return camera.isOpened(); }
int Camera::brightness() { return this->camera.get(cv::CAP_PROP_BRIGHTNESS); }
int Camera::contrast() { return this->camera.get(cv::CAP_PROP_CONTRAST); }
int Camera::saturation() { return this->camera.get(cv::CAP_PROP_SATURATION); }
int Camera::gain() { return this->camera.get(cv::CAP_PROP_GAIN); }
bool Camera::backlight() { return this->camera.get(cv::CAP_PROP_BACKLIGHT) > 0; }
bool Camera::autoExposure() { return this->camera.get(cv::CAP_PROP_AUTO_EXPOSURE) > 0; }
int Camera::fps() { return _fps; } //this->camera.get(cv::CAP_PROP_FPS); } /// TODO: Unsure whether to return the cap's fps or the fps set by the user
int Camera::frameWidth() { return this->camera.get(cv::CAP_PROP_FRAME_WIDTH); }
int Camera::frameHeight() { return this->camera.get(cv::CAP_PROP_FRAME_HEIGHT); }
int Camera::hue() { return this->camera.get(cv::CAP_PROP_HUE); }
int Camera::exposure() { return this->camera.get(cv::CAP_PROP_EXPOSURE); }
int Camera::sharpness() { return this->camera.get(cv::CAP_PROP_SHARPNESS); }
int Camera::gamma() { return this->camera.get(cv::CAP_PROP_GAMMA); }
int Camera::bitrate() { return this->camera.get(cv::CAP_PROP_BITRATE); }

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
        emit dataReady(data);
    }
}

void Camera::setBrightness(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_BRIGHTNESS, value);
		emit brightnessChanged(value);
    }
}

void Camera::setContrast(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_CONTRAST, value);
		emit contrastChanged(value);
    }
}

void Camera::setSaturation(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_SATURATION, value);
		emit saturationChanged(value);
    }
}

void Camera::setGain(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_GAIN, value);
		emit gainChanged(value);
    }
}

void Camera::setBacklight(bool value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_BACKLIGHT, value);
		emit backlightChanged(value);
    }
}

void Camera::setAutoExposure(bool value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_AUTO_EXPOSURE, value);
		emit autoExposureChanged(value);
    }
}

void Camera::setFPS(int fps) {
    if (_fps != fps) {
        _fps = fps;
        frameTimer->setInterval(static_cast<int>(1000 / fps));
        emit fpsChanged(fps);
    }

    /// TODO: Figure out how to combine the CAP_PROP stuff or just get rid of it entirely
    if (isOpened()) {
        camera.set(cv::CAP_PROP_FPS, fps);
    }
}

void Camera::setFrameWidth(int width) {
    if (isOpened()) {
		camera.set(cv::CAP_PROP_FRAME_WIDTH, width);
		emit frameWidthChanged(width);
    }
}

void Camera::setFrameHeight(int height) {
	if (isOpened()) {
		camera.set(cv::CAP_PROP_FRAME_HEIGHT, height);
		emit frameHeightChanged(height);
	}
}

void Camera::setHue(int value) {
    if (isOpened()) {
		camera.set(cv::CAP_PROP_HUE, value);
		emit hueChanged(value);
    }
}

void Camera::setExposure(int value) {
	if (isOpened()) {
		camera.set(cv::CAP_PROP_EXPOSURE, value);
		emit exposureChanged(value);
	}
}

void Camera::setSharpness(int value) {
	if (isOpened()) {
		camera.set(cv::CAP_PROP_SHARPNESS, value);
		emit sharpnessChanged(value);
	}
}

void Camera::setGamma(int value) {
	if (isOpened()) {
		camera.set(cv::CAP_PROP_GAMMA, value);
		emit gammaChanged(value);
	}
}

void Camera::setBitrate(int value) {
	if (isOpened()) {
		camera.set(cv::CAP_PROP_BITRATE, value);
		emit bitrateChanged(value);
	}
}

void Camera::restart() {
    stop();
    start();
}

void Camera::setVideoDevice(int deviceIndex) {
    // Set new index
	_videoDeviceIndex = deviceIndex;
	emit deviceChanged();
}