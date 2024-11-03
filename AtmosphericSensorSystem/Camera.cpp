#include "Camera.h"

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

Camera::Camera(QObject *parent)
	: QObject(parent), frameTimer(new QTimer())
{}

Camera::~Camera()
{}

void Camera::open(int deviceIndex) {
    camera.open(deviceIndex);
}

void Camera::release() {
    camera.release();
}

bool Camera::isOpened() {
    return camera.isOpened();
}

QUrl& Camera::getOutputDirectory() {
    return outputDir;
}

QTimer* Camera::getFrameTimer() {
    return frameTimer;
}

CameraState& Camera::getState() {
    return state;
}

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
    }
}

void Camera::setContrast(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_CONTRAST, value);
    }
}

void Camera::setSaturation(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_SATURATION, value);
    }
}

void Camera::setGain(int value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_GAIN, value);
    }
}

void Camera::setBacklight(bool value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_BACKLIGHT, value);
    }
}

void Camera::setAutoExposure(bool value) {
    if (isOpened()) {
        camera.set(cv::CAP_PROP_AUTO_EXPOSURE, value);
    }
}

void Camera::setVideoDevice() {

}

void Camera::restart() {
    stop();
    start();
}

void Camera::start() {
    if (isOpened()) {
        frameTimer->start(30);  // Adjust the interval for desired frame rate
    }
}

void Camera::stop() {
    if (frameTimer) {
        frameTimer->stop();
    }
    
    if (isOpened()) {
        release();
    }
}

void Camera::startRecording() {
    /// TODO: Maybe rename this from "record" or make a "toggleRecording" slot
    if (state == CAMERA_RECORDING) {
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
                state = CAMERA_RECORDING;
            }
            else {
                /// TODO: Signal a camera error
            }
        }
    }
}

void Camera::pauseRecording() {
    /// TODO: Signal recording stopped
}

void Camera::stopRecording() {
    videoWriter.release(); // Stop recording
    state = CAMERA_IDLE;
    /// TODO: Signal recording stopped
}

void Camera::setOutputDirectory(QUrl directory) {
    outputDir = directory;
}