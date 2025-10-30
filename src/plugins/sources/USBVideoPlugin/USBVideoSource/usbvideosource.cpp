#include "usbvideosource.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <utils/boost_qt_conversions.h>

USBVideoSource::USBVideoSource(QByteArray hardwareId, QObject* parent)
	: USBVideoSource(getCameraDevice(hardwareId), parent)
{
    m_id = hardwareId;
}

USBVideoSource::USBVideoSource(QCameraDevice qVideoDevice, QObject* parent)
	: Source(parent), mCamera(qVideoDevice)
{
    m_id = qVideoDevice.id();
    m_name = qVideoDevice.description();
	m_pluginId = "usb_video";
    mSourceType = Source::Type::VIDEO;
    pPreview.reset(new USBVideoPreview(&mSession, boostUuidToQUuid(this->uuid()), this));

    // Initialize capture session
    mSession.setCamera(&mCamera);
    mSession.setRecorder(&mRecorder);

    // Init recorder
    mRecorder.setOutputLocation(QDir::currentPath() + "/output");
}

USBVideoSource::~USBVideoSource()
{
    // Release the camera
    if (mCamera.isActive()) {
        stop();
    }
}

void USBVideoSource::open() {
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

    // Init preview
	if (!pPreview) pPreview.reset(new USBVideoPreview(&mSession, boostUuidToQUuid(this->uuid()), this));

    emit previewAvailable(this, pPreview.get());
}

void USBVideoSource::start() {
    if (mState == STARTED) return;

    // Check if the camera is available/is idle
    if (!mCamera.isAvailable() || mCamera.isActive()) return; // TODO: Do more logging here

    // Open the camera
    mCamera.start();

    // Emit signal
    emit started();
}

void USBVideoSource::stop() {
    if (mState == STOPPED) return;

    // Check if camera is good to go/is active
    if (!mCamera.isAvailable() || !mCamera.isActive()) return; // TODO: Do more logging here
    
    // Stop camera
    mCamera.stop();

    // Emit signal
    emit stopped();
}

void USBVideoSource::close()
{
    if (mState == CLOSED || mState == STOPPED) return;
	// TODO: Implement close
}

void USBVideoSource::onNewFrame(const QVideoFrame& frame) {
	// Emit signal with the frame
	emit frameReady(frame);
}

void USBVideoSource::setMediaDirectory(QUrl directory)
{
	mRecorder.setOutputLocation(directory);
	emit mediaDirectoryChanged(directory);
}

QCameraDevice USBVideoSource::getCameraDevice(const QByteArray& id)
{
	// Get the camera source from the id
	for (auto videoDevice : QMediaDevices::videoInputs()) {
		if (videoDevice.id() == id) {
			return videoDevice;
		}
	}
    
	// If not found, return an empty camera source
    return QCameraDevice();
}

QWidget* USBVideoSource::createConfigWidget(QWidget* parent)
{
	// Avoid linking to app's SourcePropertiesWidget. Build a simple tabbed widget locally.
	QWidget* container = new QWidget(parent);
	auto* layout = new QVBoxLayout(container);
	layout->setContentsMargins(0, 0, 0, 0);

	auto* tabs = new QTabWidget(container);
	tabs->addTab(new USBVideoPropertiesWidget(this, tabs), QStringLiteral("Source"));
	tabs->addTab(new USBVideoRecordingPropertiesWidget(tabs, &mRecorder), QStringLiteral("Recording"));

	layout->addWidget(tabs);
	return container;
}

void USBVideoSource::loadSettings(const QJsonObject& obj)
{
	QJsonObject source = obj["source"].toObject();
	QJsonObject recording = obj["recording"].toObject();

    // Load source categories
	QJsonObject deviceFormat = source["format"].toObject();
	QJsonObject deviceZoomAndFocus = source["zoom_and_focus"].toObject();
	QJsonObject deviceOther = source["other"].toObject();

	// Load source format
	QCameraFormat format;
    auto availableFormats = mCamera.cameraDevice().videoFormats();
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.maxFrameRate() == deviceFormat["max_fps"].toDouble() &&
			availableFormat.minFrameRate() == deviceFormat["min_fps"].toDouble() &&
			availableFormat.resolution().width() == deviceFormat["width"].toInt() &&
			availableFormat.resolution().height() == deviceFormat["height"].toInt() &&
			availableFormat.pixelFormat() == static_cast<QVideoFrameFormat::PixelFormat>(deviceFormat["pixel_format"].toInt())) {
			format = availableFormat;
			break;
		}
	}

	if (format.isNull()) {
		qWarning() << "No matching format found for the camera.";
	}

	// Load zoom and focus
	mCamera.setZoomFactor(deviceZoomAndFocus["zoom_factor"].toDouble());
	//mCamera.setMaximumZoomFactor(deviceZoomAndFocus["max_zoom_factor"].toDouble());
	//mCamera.setMinimumZoomFactor(deviceZoomAndFocus["min_zoom_factor"].toDouble());
	mCamera.setFocusMode(static_cast<QCamera::FocusMode>(deviceZoomAndFocus["focus_mode"].toInt()));
	mCamera.setFocusDistance(deviceZoomAndFocus["focus_distance"].toDouble());
   
	// Load other settings
	mCamera.setColorTemperature(deviceOther["color_temperature"].toInt());
	mCamera.setExposureMode(static_cast<QCamera::ExposureMode>(deviceOther["exposure_mode"].toInt()));
	mCamera.setExposureCompensation(deviceOther["exposure_compensation"].toDouble());
	//mCamera.setExposureTime(deviceOther["exposure_time"].toDouble());
	mCamera.setFlashMode(static_cast<QCamera::FlashMode>(deviceOther["flash_mode"].toInt()));
	mCamera.setManualExposureTime(deviceOther["manual_exposure_time"].toDouble());
	mCamera.setManualIsoSensitivity(deviceOther["manual_iso"].toDouble());
	//mCamera.setIsoSensitivity(deviceOther["iso"].toDouble());
	mCamera.setTorchMode(static_cast<QCamera::TorchMode>(deviceOther["torch_mode"].toInt()));
	mCamera.setWhiteBalanceMode(static_cast<QCamera::WhiteBalanceMode>(deviceOther["white_balance_mode"].toInt()));

	// Load recording sections
	QJsonObject recordingGeneral = recording["general"].toObject();
	QJsonObject recordingVideo = recording["video"].toObject();

	// Load general recording properties
	mRecorder.setOutputLocation(QUrl::fromLocalFile(recordingGeneral["output_location"].toString()));
	mRecorder.setAutoStop(recordingGeneral["auto_stop"].toBool());
	mRecorder.setQuality(static_cast<QMediaRecorder::Quality>(recordingGeneral["quality"].toInt()));
	mRecorder.setEncodingMode(static_cast<QMediaRecorder::EncodingMode>(recordingGeneral["encoding_mode"].toInt()));
	mRecorder.mediaFormat().setFileFormat(static_cast<QMediaFormat::FileFormat>(recordingGeneral["file_format"].toInt()));

	// Load video recording properties
	mRecorder.mediaFormat().setVideoCodec(static_cast<QMediaFormat::VideoCodec>(recordingVideo["codec"].toInt()));
	mRecorder.setVideoBitRate(recordingVideo["bit_rate"].toInt());
	mRecorder.setVideoFrameRate(recordingVideo["frame_rate"].toDouble());
	mRecorder.setVideoResolution(QSize(recordingVideo["width"].toInt(), recordingVideo["height"].toInt()));

}

QJsonObject USBVideoSource::saveSettings()
{
    // Save source format
    QJsonObject deviceFormat;
    deviceFormat["max_fps"] = mCamera.cameraFormat().maxFrameRate();
    deviceFormat["min_fps"] = mCamera.cameraFormat().minFrameRate();
    deviceFormat["width"] = mCamera.cameraFormat().resolution().width();
    deviceFormat["height"] = mCamera.cameraFormat().resolution().height();
    deviceFormat["pixel_format"] = mCamera.cameraFormat().pixelFormat();

    // Save zoom and focus
    QJsonObject deviceZoomAndFocus;
    //propertiesZoomAndFocus["zoom_rate"] = mCamera.zoom;
    deviceZoomAndFocus["zoom_factor"] = mCamera.zoomFactor();
    deviceZoomAndFocus["max_zoom_factor"] = mCamera.maximumZoomFactor();
    deviceZoomAndFocus["min_zoom_factor"] = mCamera.minimumZoomFactor();
    deviceZoomAndFocus["focus_mode"] = mCamera.focusMode();
    deviceZoomAndFocus["focus_distance"] = mCamera.focusDistance();

    // Save other settings
    QJsonObject deviceOther;
    deviceOther["color_temperature"] = mCamera.colorTemperature();
    deviceOther["exposure_mode"] = mCamera.exposureMode();
    deviceOther["exposure_compensation"] = mCamera.exposureCompensation();
    deviceOther["exposure_time"] = mCamera.exposureTime();
    deviceOther["flash_mode"] = mCamera.flashMode();
    deviceOther["manual_exposure_time"] = mCamera.manualExposureTime();
    deviceOther["manual_iso"] = mCamera.manualIsoSensitivity();
    deviceOther["iso"] = mCamera.isoSensitivity();
    deviceOther["torch_mode"] = mCamera.torchMode();
    deviceOther["white_balance_mode"] = mCamera.whiteBalanceMode();

    // Save source properties
    QJsonObject source;
    source["format"] = deviceFormat;
    source["zoom_and_focus"] = deviceZoomAndFocus;
    source["other"] = deviceOther;

    // General recording properties
    QJsonObject recordingGeneral;
    recordingGeneral["output_location"] = mRecorder.outputLocation().toLocalFile();
    recordingGeneral["auto_stop"] = mRecorder.autoStop();
    recordingGeneral["quality"] = mRecorder.quality();
    recordingGeneral["encoding_mode"] = mRecorder.encodingMode();
    recordingGeneral["file_format"] = mRecorder.mediaFormat().fileFormat();

    // Video recording properties
	QJsonObject recordingVideo;
    recordingVideo["codec"] = static_cast<int>(mRecorder.mediaFormat().videoCodec());
	recordingVideo["bit_rate"] = mRecorder.videoBitRate();
	recordingVideo["frame_rate"] = mRecorder.videoFrameRate();
	recordingVideo["width"] = mRecorder.videoResolution().width();
	recordingVideo["height"] = mRecorder.videoResolution().height();
	
    // Save recording properties
    QJsonObject recording;
	recording["general"] = recordingGeneral;
	recording["video"] = recordingVideo;

	// Save all properties
    QJsonObject obj;
	obj["source"] = source;
	obj["recording"] = recording;

    return obj;
}

void USBVideoSource::restart() {
    stop();
    start();
}

