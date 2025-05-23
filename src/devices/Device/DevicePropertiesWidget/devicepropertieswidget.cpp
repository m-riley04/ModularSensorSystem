#include "devicepropertieswidget.h"

DevicePropertiesWidget::DevicePropertiesWidget(Device* device, QWidget *parent)
	: QWidget(parent), pDevice(device)
{
	// Init button frame
	pFrameButtons = new QFrame(this);
	pFrameButtons->setFrameShape(QFrame::StyledPanel);
	pButtonLayout = new QVBoxLayout(pFrameButtons);
	pButtonLayout->setAlignment(Qt::AlignTop);
	pButtonLayout->setSpacing(4);
	pButtonLayout->setContentsMargins(6, 6, 6, 6);
	pFrameButtons->setFixedWidth(140);

	// Init page stack
	pStackPages = new QStackedWidget(this);
	pStackPages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Set layout
	auto* mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(pFrameButtons);
	mainLayout->addWidget(pStackPages, 1);
}

DevicePropertiesWidget::~DevicePropertiesWidget()
{}

void DevicePropertiesWidget::addPage(const QString& title, QWidget* page)
{
	auto* button = new QPushButton(title);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	pButtonLayout->addWidget(button);              // <-- real placement

	pStackPages->addWidget(page);

	connect(button, &QPushButton::clicked,
		this, [this, page] { pStackPages->setCurrentWidget(page); });
}