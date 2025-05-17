#include "previewcontainerwidget.h"

PreviewContainerWidget::PreviewContainerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PreviewContainerWidget::~PreviewContainerWidget()
{}

void PreviewContainerWidget::initSignals()
{
	if (pController == nullptr) {
		qDebug() << "Cannot initialize signals: main controller is null";
		return;
	}

	DeviceController* deviceController = pController->deviceController();
	if (deviceController == nullptr) {
		qDebug() << "Cannot initialize signals: device controller is null";
		return;
	}

	// Device added ui updates
	connect(deviceController, &DeviceController::deviceAdded, this, &PreviewContainerWidget::addDeviceWidget);
	connect(deviceController, &DeviceController::deviceRemoved, this, &PreviewContainerWidget::removeDeviceWidget);
}


void PreviewContainerWidget::addDeviceWidget(Device* device)
{
	if (device == nullptr) {
		qDebug() << "Cannot add device widget: device is null";
		return;
	}

	// Init widget ptr and tab display stuff
	CustomSinkWidget* widget = nullptr;
	QString tabName = device->name();

	// Check what type of device it is
	auto type = device->deviceType();
	switch (type) {
	case Device::Type::CAMERA:
		widget = addVideoWidget(static_cast<CameraDevice*>(device));
		break;
	default:
		widget = new CustomSinkWidget(device, this);
		break;
	}

	// Null check
	if (widget == nullptr) {
		qDebug() << "Failed to create widget for device";
		return;
	}

	// Add tab
	ui.tabDevicePreviews->addTab(widget, tabName);

	// Emit signal
	emit deviceWidgetAdded(widget);
}

void PreviewContainerWidget::removeDeviceWidget(Device* device)
{
	if (device == nullptr) {
		qDebug() << "Cannot remove device widget: device is null";
		return;
	}

	// Find the video widget for the device
	for (int i = 0; i < mVideoWidgets.size(); ++i) {
		CustomSinkWidget* widget = mVideoWidgets[i];
		if (widget->device() == device) {
			// Remove the video widget from the list and UI
			ui.tabDevicePreviews->removeTab(i);
			mVideoWidgets.removeAt(i);

			// TODO: Implement proper/more cleanup for the widget (if needed, READ QT DOCS ON QLIST/QTAB MEMORY)
			widget->deleteLater();

			emit deviceWidgetRemoved(widget); /// CONSIDER: Returning something OTHER than a pointer to the widget (since it's being deleted)
			break;
		}
	}
}


CustomSinkWidget* PreviewContainerWidget::addVideoWidget(CameraDevice* camera)
{
	if (camera == nullptr) return nullptr;
	// Create a new video widget
	CustomSinkWidget* videoWidget = new CustomSinkWidget(camera, this);

	// Add video widget to list
	mVideoWidgets.append(videoWidget);

	// Set camera and get props
	QString name = camera->camera()->cameraDevice().description();
	camera->setVideoOutput(videoWidget);

	// Add tab to widget
	//ui.tabDevicePreviews->addTab(videoWidget, name); /// CONSIDER: Consolidate and separate the adding of tabs to the general addDeviceWidget method

	return videoWidget;
}

