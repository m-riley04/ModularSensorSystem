#include "videodevice.h"


VideoDevice::VideoDevice(QCameraDevice qVideoDevice, QObject* parent)
	: Device(parent), mCamera(qVideoDevice)
{
    mId = qVideoDevice.id();
    mName = qVideoDevice.description();
    mDeviceType = Device::Type::CAMERA;
    pPreview.reset(new VideoPreview(&mSession, this));
}

VideoDevice::~VideoDevice()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void VideoDevice::open() {
	if (mState == OPENED || mState == STARTED) return; // Already opened or started

    // TODO: Move checks up/somewhere else?
	if (!mCamera.isAvailable()) {
		QMessageBox::warning(nullptr, "Camera Error", "No camera available.");
		return;
	}

    if (mCamera.isActive()) {
        QMessageBox::warning(nullptr, "Camera Error", "Camera is already active.");
        return;
    }

    // Initialize capture session
    mSession.setCamera(&mCamera);
    mSession.setRecorder(&mRecorder);

    // Init preview
	if (!pPreview) pPreview.reset(new VideoPreview(&mSession, this));

    // Init buffer
    if (!pClipBuffer) pClipBuffer.reset(new VideoClipBuffer(2, this));

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");

    emit previewAvailable(this, pPreview.get());
}

void VideoDevice::start() {
    if (mState == STARTED) return;

    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void VideoDevice::stop() {
    if (mState == STOPPED) return;

    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

void VideoDevice::close()
{
    if (mState == CLOSED || mState == STOPPED) return;
	// TODO: Implement close
}

void VideoDevice::onNewFrame(const QVideoFrame& frame) {
	// Get the timestamp
	ClipBufferBase::time timestamp = 0; // TODO: Get the timestamp from the frame

	// Cast buffer to VideoClipBuffer
	VideoClipBuffer* pVideoBuffer = static_cast<VideoClipBuffer*>(pClipBuffer.get()); // CONSIDER: Do this differently/no casting?
    
    // Add the frame to the buffer
	pVideoBuffer->push(frame, timestamp);
}

void VideoDevice::setMediaDirectory(QUrl directory)
{
	mRecorder.setOutputLocation(directory);
	emit mediaDirectoryChanged(directory);
}

bool VideoDevice::checkCameraAvailability() {
    return QMediaDevices::videoInputs().count() > 0;
}

QWidget* VideoDevice::createConfigWidget(QWidget* parent)
{
	DevicePropertiesWidget* devicePropertiesWidget = new DevicePropertiesWidget(this, parent);
	devicePropertiesWidget->addPage("Device", new VideoDevicePropertiesWidget(this, devicePropertiesWidget));
    return devicePropertiesWidget;//new VideoDevicePropertiesWidget(this, parent);
}

void VideoDevice::loadSettings(const QJsonObject& obj)
{
    // TODO: implement
}

void VideoDevice::saveSettings(QJsonObject& obj) const
{
    // TODO: implement
}

void VideoDevice::restart() {
    stop();
    start();
}

void VideoDevice::beginRecording(RecordingSession* s)
{
	// Update recording session
	pRecordingSession = s;

    QString f = s->outputDir().filePath(mName + ".mp4"); // TODO: Change from mName to something better for file names
	mRecorder.setOutputLocation(QUrl::fromLocalFile(f));
    mRecorder.record();
}

void VideoDevice::endRecording()
{
    mRecorder.stop();
}
