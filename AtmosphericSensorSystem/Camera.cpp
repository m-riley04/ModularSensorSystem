#include "Camera.h"
#include <QtConcurrent>

static bool checkCameraAvailability() {
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

Camera::Camera(QObject *parent)
	: Sensor(parent), _frameTimer(new QTimer())
{
    // Default the camera
    if (checkCameraAvailability()) {
        setVideoDevice(0);
    }
}

Camera::~Camera()
{}

void Camera::open(int deviceIndex) {
    QtConcurrent::run([this, deviceIndex]() {
        camera.open(deviceIndex, cv::CAP_DSHOW);
        if (camera.isOpened()) {
            emit started();
        }
    });
}

void Camera::release() {
    camera.release();
}

bool Camera::isOpened() { return camera.isOpened(); }
QTimer* Camera::frameTimer() { return _frameTimer; }
int Camera::brightness() { return this->camera.get(cv::CAP_PROP_BRIGHTNESS); }
int Camera::contrast() { return this->camera.get(cv::CAP_PROP_CONTRAST); }
int Camera::saturation() { return this->camera.get(cv::CAP_PROP_SATURATION); }
int Camera::gain() { return this->camera.get(cv::CAP_PROP_GAIN); }
int Camera::exposure() { return this->camera.get(cv::CAP_PROP_EXPOSURE); }
bool Camera::backlight() { return this->camera.get(cv::CAP_PROP_BACKLIGHT) > 0; }
bool Camera::autoExposure() { return this->camera.get(cv::CAP_PROP_AUTO_EXPOSURE) > 0; }
int Camera::viewfinderFrameRate() { return _viewfinderFrameRate; }

Camera& Camera::operator >> (cv::Mat& image) {
    if (camera.isOpened()) {
        camera >> image; // Capture frame from camera
    }
    else {
        image = cv::Mat(); // Return an empty frame if the camera is not open
    }
    return *this;
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
    if (isOpened()) {
        camera.set(cv::CAP_PROP_FPS, fps);
		emit fpsChanged(fps);
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

void Camera::setViewfinderFrameRate(int value) {
    if (_viewfinderFrameRate != value) {
        _viewfinderFrameRate = value;
        _frameTimer->setInterval(value);
        emit viewfinderFrameRateChanged(value);
    }
}

void Camera::restart() {
    stop();
    start();
}

void Camera::start() {
    if (!isOpened()) {
        // Default the camera
        if (checkCameraAvailability()) {
            setVideoDevice(0);
        }
    }
    _frameTimer->start(_viewfinderFrameRate);  // Adjust the interval for desired frame rate
}

void Camera::pause() {
    // Implement the pause functionality here
}

void Camera::stop() {
    if (frameTimer) {
        _frameTimer->stop();
    }
    
    if (isOpened()) {
        release();
    }
}

void Camera::setVideoDevice(int deviceIndex) {
    // Stop the current device
    stop();

    // Open the new device
    open(deviceIndex);

    // Start the new device
    if (isOpened()) {
        start();
    }
}

void Camera::startRecording() {
    /// TODO: Maybe rename this from "record" or make a "toggleRecording" slot
    if (_state == SENSOR_RECORDING) {
        stop();
    }

    if (isOpened()) {
        /// TODO: Get filename another way
        QString filename = "test.avi";//QFileDialog::getSaveFileName(this, "Save Video", outputDir.toString(), "Video Files (*.avi *.mp4)");

        if (!filename.isEmpty()) {
            // Set up VideoWriter with file path, codec, frame rate, and frame size
            int frameWidth = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_WIDTH));
            int frameHeight = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_HEIGHT));
            double fps = 30.0; // Set desired frame rate

            // Use an appropriate codec, e.g., 'MJPG' for .avi, 'MP4V' for .mp4
            videoWriter.open(filename.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight), true);

            if (videoWriter.isOpened()) {
                _state = SENSOR_RECORDING;
            }
            else {
                /// TODO: Signal a camera error
            }
        }
    }
}

void Camera::pauseRecording() {
    /// TODO: Signal recording stopped
	emit recordingPaused();
}

void Camera::stopRecording() {
    videoWriter.release(); // Stop recording
    _state = SENSOR_IDLE;
	emit recordingStopped();
    /// TODO: Signal recording stopped
}