#include "devicepropertieswidget.h"

DevicePropertiesWidget::DevicePropertiesWidget(Device* device, QWidget *parent)
	: QWidget(parent), pDevice(device)
{
	// Init button frame
	QVBoxLayout* buttonLayout = new QVBoxLayout(this);
	pFrameButtons = new QFrame(this);
	pFrameButtons->setLayout(buttonLayout);
	pFrameButtons->setObjectName("frameButtons");
	pFrameButtons->setGeometry(QRect(0, 0, 200, 600));
	pFrameButtons->setStyleSheet("QFrame#frameButtons { background-color: #2E2E2E; }");
	pFrameButtons->setFrameShape(QFrame::StyledPanel);
	pFrameButtons->setFrameShadow(QFrame::Raised);
	pFrameButtons->setLineWidth(1);
	pFrameButtons->setMidLineWidth(1);

	// Init page stack
	pStackPages = new QStackedWidget(this);
	pStackPages->setObjectName("stackPages");
	pStackPages->setGeometry(QRect(200, 0, 600, 600));
	pStackPages->setStyleSheet("QStackedWidget#stackPages { background-color: #2E2E2E; }");
	pStackPages->setFrameShape(QFrame::StyledPanel);
	pStackPages->setFrameShadow(QFrame::Raised);
	pStackPages->setLineWidth(1);
	pStackPages->setMidLineWidth(1);

	// Set layout
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(pFrameButtons, 0, 0);
	mainLayout->addWidget(pStackPages, 0, 1);
}

DevicePropertiesWidget::~DevicePropertiesWidget()
{}
