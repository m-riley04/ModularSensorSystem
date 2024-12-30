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

void QtCameraControlsDialog::populateFilterDropdowns()
{
	// Populate dropdowns with all possible for each category
	QSet<float> fpsSet;
	QSet<QSize> resolutionSet;
	QSet<QVideoFrameFormat::PixelFormat> pixelFormatSet;
	for (QCameraFormat format : mFormats)
	{
		fpsSet.insert(format.maxFrameRate());
		resolutionSet.insert(format.resolution());
		pixelFormatSet.insert(format.pixelFormat());
	}

	// Sort each set
	QList<float> fpsList = fpsSet.values();
	std::sort(fpsList.begin(), fpsList.end(), std::greater<float>());
	QList<QSize> resolutionList = resolutionSet.values();
	std::sort(resolutionList.begin(), resolutionList.end(), [](const QSize &a, const QSize &b){
			return a.width() > b.width();
		});
	QList<QVideoFrameFormat::PixelFormat> pixelFormatList = pixelFormatSet.values();
	std::sort(pixelFormatList.begin(), pixelFormatList.end(), [](const QVideoFrameFormat::PixelFormat& a, const QVideoFrameFormat::PixelFormat& b) {
			return pixelFormatMap[a] > pixelFormatMap[b]; 
		});

	// Populate fps dropdown
	for (float fps : fpsList)
	{
		ui.dropdownFps->addItem(QString::number(fps), fps);
	}

	// Populate resolution dropdown
	for (QSize resolution : resolutionList)
	{
		ui.dropdownResolution->addItem(sizeToString(resolution), resolution);
	}

	// Populate pixel format dropdown
	for (QVideoFrameFormat::PixelFormat pixelFormat : pixelFormatList)
	{
		ui.dropdownPixelFormat->addItem(pixelFormatMap[pixelFormat], pixelFormat);
	}
}

void QtCameraControlsDialog::initializeFormatGroup(Camera* camera)
{
	// Initialize format list
	mFormats = camera->camera().cameraDevice().videoFormats();

	// Check if no formats are available
	if (mFormats.isEmpty())
	{
		/// TODO: Maybe put a message instead?
		ui.groupFormat->hide();
		return;
	}

	// Populate format table
	for (QCameraFormat format : mFormats)
	{
		ui.tableFormats->insertRow(ui.tableFormats->rowCount());
		int row = ui.tableFormats->rowCount() - 1;
		ui.tableFormats->setItem(row, 0, new QtCameraFormatTableWidgetItem(QString::number(format.maxFrameRate()), QtCameraFormatTableItemType::FPS)); // FPS
		ui.tableFormats->setItem(row, 1, new QtCameraFormatTableWidgetItem(sizeToString(format.resolution()), QtCameraFormatTableItemType::RESOLUTION)); // Resolution
		ui.tableFormats->setItem(row, 2, new QtCameraFormatTableWidgetItem(pixelFormatMap[format.pixelFormat()], QtCameraFormatTableItemType::PIXEL_FORMAT)); // Pixel Format
	}

	// Get default format
	defaultFormat = mFormats.first();

	// Update ui elements
	ui.labelFps->setText(QString::number(defaultFormat.maxFrameRate()));
	ui.labelResolution->setText(sizeToString(defaultFormat.resolution()));
	ui.labelPixelFormat->setText(pixelFormatMap[defaultFormat.pixelFormat()]);

	// Populate filter dropdowns
	resetFilters();
	
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
	// Initialize filters
	connect(ui.dropdownFps, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onFilterChanged);
	connect(ui.dropdownResolution, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onFilterChanged);
	connect(ui.dropdownPixelFormat, &QComboBox::currentIndexChanged, this, &QtCameraControlsDialog::onFilterChanged);

	// Initialize reset button
	connect(ui.buttonResetFilters, &QPushButton::clicked, this, &QtCameraControlsDialog::resetFilters);
}

void QtCameraControlsDialog::resetFilters()
{
	// Block signals to prevent infinite loops
	ui.dropdownFps->blockSignals(true);
	ui.dropdownResolution->blockSignals(true);
	ui.dropdownPixelFormat->blockSignals(true);

	// Clear dropdowns
	ui.dropdownFps->clear();
	ui.dropdownResolution->clear();
	ui.dropdownPixelFormat->clear();

	// Populate dropdowns
	populateFilterDropdowns();

	// Unblock signals
	ui.dropdownFps->blockSignals(false);
	ui.dropdownResolution->blockSignals(false);
	ui.dropdownPixelFormat->blockSignals(false);
}

void QtCameraControlsDialog::onFilterChanged()
{
	// Get all filter values
	QString fpsString = ui.dropdownFps->currentText();
	QString resolutionString = ui.dropdownResolution->currentText();
	QString pixelFormatString = ui.dropdownPixelFormat->currentText();

	// Repopulate resolution and pixel format dropdowns
	
}
