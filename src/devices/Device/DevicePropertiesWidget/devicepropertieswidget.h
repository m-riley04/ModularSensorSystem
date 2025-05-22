#pragma once

#include <QWidget>
#include <QtWidgets>
#include "devices/Device/device.h"

class Device;

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
		auto button = new QPushButton(title, pFrameButtons);
		pStackPages->addWidget(page);

		// Connect button to page
		connect(button, &QPushButton::clicked, [this, page]() {
			pStackPages->setCurrentWidget(page);
			});
	}

protected:
	Device* pDevice = nullptr;
	QFrame* pFrameButtons = nullptr;
	QStackedWidget* pStackPages = nullptr;
	
};
