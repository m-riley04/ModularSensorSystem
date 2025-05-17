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

	auto* widget = new DevicePreviewWidget(device->preview(), ui.tabDevicePreviews);

	// Null check
	if (widget == nullptr) {
		qDebug() << "Failed to create widget for device";
		return;
	}

	// Add widget to list
	mDevicePreviewWidgets.append(widget);

	// Add tab
	QString tabName = device->name();
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
	for (int i = 0; i < mDevicePreviewWidgets.size(); ++i) {
		DevicePreviewWidget* widget = mDevicePreviewWidgets[i];
		if (widget->preview()->device() == device) {
			// Remove the widget from the list and UI
			ui.tabDevicePreviews->removeTab(i);
			mDevicePreviewWidgets.removeAt(i);

			// TODO: Implement proper/more cleanup for the widget (if needed, READ QT DOCS ON QLIST/QTAB MEMORY)
			widget->deleteLater();

			emit deviceWidgetRemoved(widget); /// CONSIDER: Returning something OTHER than a pointer to the widget (since it's being deleted)
			break;
		}
	}
}