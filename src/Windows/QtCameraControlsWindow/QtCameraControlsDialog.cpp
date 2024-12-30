#include "QtCameraControlsDialog.h"
#include <QListWidget>

/**
* A map of pixel formats to their string representations.
*/
QMap<QVideoFrameFormat::PixelFormat, QString> pixelFormatMap{
	{ QVideoFrameFormat::Format_Invalid, "Invalid" },
	{ QVideoFrameFormat::Format_ARGB8888, "ARGB8888" },
	{ QVideoFrameFormat::Format_ARGB8888_Premultiplied, "ARGB8888_Premultiplied" },
	{ QVideoFrameFormat::Format_XRGB8888, "RGB8888" },
	{ QVideoFrameFormat::Format_BGRA8888, "BGRA8888" },
	{ QVideoFrameFormat::Format_BGRA8888_Premultiplied, "BGRA8888_Premultiplied" },
	{ QVideoFrameFormat::Format_ABGR8888, "ABGR8888" },
	{ QVideoFrameFormat::Format_ABGR8888, "ABGR8888_Premultiplied" },
	{ QVideoFrameFormat::Format_XBGR8888, "XBGR8888" },
	{ QVideoFrameFormat::Format_RGBA8888, "RGBA8888" },
	{ QVideoFrameFormat::Format_BGRX8888, "BGRX8888" },
	{ QVideoFrameFormat::Format_RGBX8888, "RGBX8888" },
	{ QVideoFrameFormat::Format_AYUV, "AYUV" },
	{ QVideoFrameFormat::Format_AYUV_Premultiplied, "AYUV_Premultiplied" },
	{ QVideoFrameFormat::Format_YUV420P, "YUV420P" },
	{ QVideoFrameFormat::Format_YUV422P, "YUV422P" },
	{ QVideoFrameFormat::Format_YV12, "YV12" },
	{ QVideoFrameFormat::Format_UYVY, "UYVY" },
	{ QVideoFrameFormat::Format_YUYV, "YUYV" },
	{ QVideoFrameFormat::Format_NV12, "NV12" },
	{ QVideoFrameFormat::Format_NV21, "NV21" },
	{ QVideoFrameFormat::Format_IMC1, "IMC1" },
	{ QVideoFrameFormat::Format_IMC2, "IMC2" },
	{ QVideoFrameFormat::Format_IMC3, "IMC3" },
	{ QVideoFrameFormat::Format_IMC4, "IMC4" },
	{ QVideoFrameFormat::Format_P010, "P010" },
	{ QVideoFrameFormat::Format_P016, "P016" },
	{ QVideoFrameFormat::Format_Y8, "Y8" },
	{ QVideoFrameFormat::Format_Y16, "Y16" },
	{ QVideoFrameFormat::Format_Jpeg, "Jpeg" },
	{ QVideoFrameFormat::Format_SamplerExternalOES, "SamplerExternalOES" },
	{ QVideoFrameFormat::Format_SamplerRect, "SamplerRect" },
	{ QVideoFrameFormat::Format_YUV420P10, "YUV420P10" },
};

QtCameraControlsDialog::QtCameraControlsDialog(Camera* camera, QWidget* parent)
	: QDialog(parent), pCamera(camera)
{
	ui.setupUi(this);

	/// Initialzie camera device
	ui.labelCameraDevice->setText(camera->camera().cameraDevice().description());
	
	/// Initialize Formats box
	initializeFormatGroup(camera);

	/// Initialize settings group
	initializeSettingsGroup(camera);
}

QtCameraControlsDialog::~QtCameraControlsDialog()
{}

QString QtCameraControlsDialog::sizeToString(QSize size)
{
	return QString::number(size.width()) + "x" + QString::number(size.height());
}

QSize QtCameraControlsDialog::stringToSize(const QString& str)
{
	QStringList parts = str.split('x');
	if (parts.size() == 2)
		return QSize(parts[0].toInt(), parts[1].toInt());
	return QSize(); // invalid if parsing failed
}

void QtCameraControlsDialog::initializeFormatGroup(Camera* camera)
{
	// Initialize format list
	mFormats = camera->camera().cameraDevice().videoFormats();

	if (mFormats.isEmpty())
	{
		ui.groupFormat->hide();
		return;
	}

	// Get default format
	defaultFormat = mFormats.first();

	// Get initial resolution and pixel format
	QString resolution = sizeToString(defaultFormat.resolution());
	QVideoFrameFormat::PixelFormat pixelFormat = defaultFormat.pixelFormat();

	// Populate dropdowns
	ui.dropdownFps->blockSignals(true);
	ui.dropdownResolution->blockSignals(true);
	ui.dropdownPixelFormat->blockSignals(true);

	selectedFps = populateFpsDropdown(resolution, pixelFormat);
	selectedResolution = populateResolutionDropdown(selectedFps, pixelFormat);
	selectedPixelFormat = populatePixelFormatDropdown(selectedFps, selectedResolution);

	ui.dropdownFps->blockSignals(false);
	ui.dropdownResolution->blockSignals(false);
	ui.dropdownPixelFormat->blockSignals(false);

	// Connect control signals
	connectFormatControls();
}

void QtCameraControlsDialog::initializeSettingsGroup(Camera* camera)
{
	/// Initialize Settings Box values
	// Controls
	ui.checkboxActive->setChecked(camera->camera().isActive());
	ui.sliderColorTemp->setValue(camera->camera().colorTemperature());
	ui.dropdownExposureMode->setCurrentIndex(static_cast<int>(camera->camera().exposureMode()));
	ui.sliderExposureComp->setValue(camera->camera().exposureCompensation());
	ui.dropdownFlashMode->setCurrentIndex(static_cast<int>(camera->camera().flashMode()));
	ui.checkboxAutoExposureTime->setChecked(camera->camera().exposureTime());
	ui.sliderManualExposureTime->setValue(camera->camera().manualExposureTime());
	ui.checkboxAutoIsoSensitivity->setChecked(camera->camera().isoSensitivity());
	ui.sliderManualIsoSensitivity->setValue(camera->camera().manualIsoSensitivity());
	ui.dropdownTorchMode->setCurrentIndex(static_cast<int>(camera->camera().torchMode()));
	ui.dropdownWhiteBalanceMode->setCurrentIndex(static_cast<int>(camera->camera().whiteBalanceMode()));

	/// Initialize min/max values
	//ui.sliderColorTemp->setMinimum();
	//ui.sliderColorTemp->setMaximum();
	//ui.sliderExposureComp->setMinimum();
	//ui.sliderExposureComp->setMaximum();
	ui.sliderManualExposureTime->setMinimum(camera->camera().minimumExposureTime());
	ui.sliderManualExposureTime->setMaximum(camera->camera().maximumExposureTime());
	ui.sliderManualIsoSensitivity->setMinimum(camera->camera().minimumIsoSensitivity());
	ui.sliderManualIsoSensitivity->setMaximum(camera->camera().maximumIsoSensitivity());

	// Connect widgets to controls
	connectPropertyControls(camera);
}


int QtCameraControlsDialog::populateFpsDropdown(QString resolution, QVideoFrameFormat::PixelFormat pixelFormat)
{
	QList<QCameraFormat> fpsFormatList = QtConcurrent::blockingFiltered(mFormats, [resolution, pixelFormat](QCameraFormat format) {
		return format.pixelFormat() == pixelFormat && sizeToString(format.resolution()) == resolution;
		});

	ui.dropdownFps->clear();
	for (QCameraFormat format : fpsFormatList)
	{
		ui.dropdownFps->addItem(QString::number(format.maxFrameRate()), format.maxFrameRate());
	}

	// Select first item
	if (!fpsFormatList.isEmpty()) {
		ui.dropdownFps->setCurrentIndex(0);
		return fpsFormatList.first().maxFrameRate();
	}
	
	return 0.0;
}

QString QtCameraControlsDialog::populateResolutionDropdown(float fps, QVideoFrameFormat::PixelFormat pixelFormat)
{
	QList<QCameraFormat> resolutionFormatList = QtConcurrent::blockingFiltered(mFormats, [fps, pixelFormat](QCameraFormat format) {
		return format.pixelFormat() == pixelFormat && format.maxFrameRate() == fps;
		});
	ui.dropdownResolution->clear();
	for (QCameraFormat format : resolutionFormatList)
	{
		ui.dropdownResolution->addItem(sizeToString(format.resolution()), format.resolution());
	}

	if (!resolutionFormatList.isEmpty()) {
		ui.dropdownResolution->setCurrentIndex(0);
		return sizeToString(resolutionFormatList.first().resolution());
	}

	return "";
}

QVideoFrameFormat::PixelFormat QtCameraControlsDialog::populatePixelFormatDropdown(float fps, QString resolution)
{
	QList<QCameraFormat> pixelFormatList = QtConcurrent::blockingFiltered(mFormats, [fps, resolution](QCameraFormat format) {
		return sizeToString(format.resolution()) == resolution && format.maxFrameRate() == fps;
		});
	ui.dropdownPixelFormat->clear();

	for (QCameraFormat format : pixelFormatList)
	{
		ui.dropdownPixelFormat->addItem(pixelFormatMap[format.pixelFormat()], format.pixelFormat());
	}

	if (!pixelFormatList.isEmpty())
	{
		ui.dropdownPixelFormat->setCurrentIndex(0);
		return pixelFormatList.first().pixelFormat(); 
	}

	/// TODO: Maybe change this
	return QVideoFrameFormat::Format_Invalid;
}

void QtCameraControlsDialog::connectPropertyControls(Camera* camera)
{
	connect(ui.checkboxActive, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->camera().setActive(state == Qt::CheckState::Checked);
		});
	connect(ui.sliderColorTemp, &QSlider::valueChanged, camera, [camera](int value) {
		camera->camera().setColorTemperature(value);
		});
	connect(ui.dropdownExposureMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->camera().setExposureMode(static_cast<QCamera::ExposureMode>(index));
		});
	connect(ui.sliderExposureComp, &QSlider::valueChanged, camera, [camera](int value) {
		camera->camera().setExposureCompensation(value);
		});
	connect(ui.dropdownFlashMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->camera().setFlashMode(static_cast<QCamera::FlashMode>(index));
		});
	connect(ui.checkboxAutoExposureTime, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->camera().setAutoExposureTime(); // TODO: add functionality
		});
	connect(ui.sliderManualExposureTime, &QSlider::valueChanged, camera, [camera](int value) {
		camera->camera().setManualExposureTime(value);
		});
	connect(ui.checkboxAutoIsoSensitivity, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->camera().setAutoIsoSensitivity(); // TODO: add functionality
		});
	connect(ui.sliderManualIsoSensitivity, &QSlider::valueChanged, camera, [camera](int value) {
		camera->camera().setManualIsoSensitivity(value);
		});
	connect(ui.dropdownTorchMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->camera().setTorchMode(static_cast<QCamera::TorchMode>(index));
		});
	connect(ui.dropdownWhiteBalanceMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->camera().setWhiteBalanceMode(static_cast<QCamera::WhiteBalanceMode>(index));
		});
}

void QtCameraControlsDialog::connectFormatControls()
{
	connect(ui.dropdownFps, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onFpsChanged);
	connect(ui.dropdownResolution, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onResolutionChanged);
	connect(ui.dropdownPixelFormat, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onPixelFormatChanged);
	
	/// Initialize view all formats
	connect(ui.buttonSeeAllFormats, &QPushButton::clicked, [this]() {
		// Create a new dialog
		QDialog* dialog = new QDialog(this);
		dialog->setWindowTitle("Supported Formats");
		QVBoxLayout* layout = new QVBoxLayout(dialog);
		dialog->setLayout(layout);
		// Create a list widget
		QListWidget* list = new QListWidget(dialog);
		layout->addWidget(list);
		// Add items to the list
		for (QCameraFormat format : mFormats)
		{
			QListWidgetItem* item = new QListWidgetItem(list);
			item->setText(QString("Resolution: %1, FPS: %2, Pixel Format: %3")
				.arg(sizeToString(format.resolution()))
				.arg(format.maxFrameRate())
				.arg(pixelFormatMap[format.pixelFormat()]));
		}
		// Show the dialog
		dialog->show();
		});

	// Initialize reset button
	connect(ui.buttonResetDropdowns, &QPushButton::clicked, this, &QtCameraControlsDialog::resetFormatDropdowns);
}

void QtCameraControlsDialog::resetFormatDropdowns()
{
	// Block signals to prevent infinite loops
	ui.dropdownFps->blockSignals(true);
	ui.dropdownResolution->blockSignals(true);
	ui.dropdownPixelFormat->blockSignals(true);

	// Clear dropdowns
	ui.dropdownFps->clear();
	ui.dropdownResolution->clear();
	ui.dropdownPixelFormat->clear();

	// Reset selected values
	selectedFps = 0.0;
	selectedResolution = "";
	selectedPixelFormat = QVideoFrameFormat::Format_Invalid;

	// Populate dropdowns with all possible for each category
	QSet<float> fpsSet;
	QSet<QString> resolutionSet;
	QSet<QVideoFrameFormat::PixelFormat> pixelFormatSet;
	for (QCameraFormat format : mFormats)
	{
		fpsSet.insert(format.maxFrameRate());
		resolutionSet.insert(sizeToString(format.resolution()));
		pixelFormatSet.insert(format.pixelFormat());
	}

	// Populate fps dropdown
	for (float fps : fpsSet)
	{
		ui.dropdownFps->addItem(QString::number(fps), fps);
	}

	// Populate resolution dropdown
	for (QString resolution : resolutionSet)
	{
		ui.dropdownResolution->addItem(resolution, resolution);
	}

	// Populate pixel format dropdown
	for (QVideoFrameFormat::PixelFormat pixelFormat : pixelFormatSet)
	{
		ui.dropdownPixelFormat->addItem(pixelFormatMap[pixelFormat], pixelFormat);
	}

	// Unblock signals
	ui.dropdownFps->blockSignals(false);
	ui.dropdownResolution->blockSignals(false);
	ui.dropdownPixelFormat->blockSignals(false);
}

void QtCameraControlsDialog::onFpsChanged(int index)
{
	// Get FPS at new index
	float fps = ui.dropdownFps->itemText(index).toFloat();

	// Repopulate resolution and pixel format dropdowns
	ui.dropdownPixelFormat->blockSignals(true);
	ui.dropdownResolution->blockSignals(true);
	selectedResolution = populateResolutionDropdown(fps, selectedPixelFormat);
	selectedPixelFormat = populatePixelFormatDropdown(fps, selectedResolution);
	ui.dropdownPixelFormat->blockSignals(false);
	ui.dropdownResolution->blockSignals(false);
}

void QtCameraControlsDialog::onResolutionChanged(int index)
{
	// Get resolution at new index
	QString resolution = ui.dropdownResolution->itemText(index);
	
	// Repopulate fps and pixel format dropdown
	ui.dropdownFps->blockSignals(true);
	ui.dropdownPixelFormat->blockSignals(true);
	selectedFps = populateFpsDropdown(resolution, selectedPixelFormat);
	selectedPixelFormat = populatePixelFormatDropdown(selectedFps, resolution);
	ui.dropdownFps->blockSignals(false);
	ui.dropdownPixelFormat->blockSignals(false);
	
}

void QtCameraControlsDialog::onPixelFormatChanged(int index)
{
	// Get pixel format at new index
	selectedPixelFormat = static_cast<QVideoFrameFormat::PixelFormat>(ui.dropdownPixelFormat->itemData(index).toInt());
	
	// Repopulate fps and resolution dropdowns
	ui.dropdownFps->blockSignals(true);
	ui.dropdownResolution->blockSignals(true);
	selectedFps = populateFpsDropdown(selectedResolution, selectedPixelFormat);
	selectedResolution = populateResolutionDropdown(selectedFps, selectedPixelFormat);
	ui.dropdownFps->blockSignals(false);
	ui.dropdownResolution->blockSignals(false);
}
