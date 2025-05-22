#pragma once

#include <QWidget>
#include "devices/Device/device.h"
#include <QPushButton>
#include "ui_devicepropertieswidget.h"

class DevicePropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	DevicePropertiesWidget(Device* device, QWidget* parent = nullptr);
	~DevicePropertiesWidget();

	Device* device() const { return pDevice; }

	void addPage(const QString& title, QWidget* page)
	{
		// Add button
		auto button = new QPushButton(title, ui.frameButtons);
		ui.stackPages->addWidget(page);

		// Connect button to page
		connect(button, &QPushButton::clicked, [this, page]() {
			ui.stackPages->setCurrentWidget(page);
			});
	}

private:
	Ui::DevicePropertiesWidgetClass ui;
	Device* pDevice = nullptr;
};
