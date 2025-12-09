#include "mountcontrolswidget.h"
#include <interfaces/capability/ipantiltmount.hpp>

MountControlsWidget::MountControlsWidget(Mount* mount, MainController& mc, QWidget *parent)
	: QWidget(parent), m_mainController(mc), m_mount(mount)
{
	ui.setupUi(this);

	// Cast to IPanTiltMount to access pan/tilt specific functionality
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) {
		// Handle error: mount does not support pan/tilt functionality
		ui.groupBox->setEnabled(false);
		ui.buttonRecenter->setEnabled(false);
		ui.buttonSetInitialAngles->setEnabled(false);
		return;
	}

	// TODO: connect signals to control the mount
	connect(ui.sliderPan, &QSlider::valueChanged, this, [panTiltMount](int value) {
		double panAngle = static_cast<double>(value);
		panTiltMount->moveTo(panAngle, panTiltMount->tiltAngle());
		});
	connect(ui.sliderTilt, &QSlider::valueChanged, this, [panTiltMount](int value) {
		double tiltAngle = static_cast<double>(value);
		panTiltMount->moveTo(panTiltMount->panAngle(), tiltAngle);
		});
	connect(ui.buttonRecenter, &QPushButton::clicked, this, [panTiltMount]() {
		panTiltMount->recenter();
		});
	connect(ui.buttonSetInitialAngles, &QPushButton::clicked, this, [this, panTiltMount]() {
		double panAngle = static_cast<double>(ui.sliderPan->value());
		double tiltAngle = static_cast<double>(ui.sliderTilt->value());
		panTiltMount->moveTo(panAngle, tiltAngle);
		});

}

MountControlsWidget::~MountControlsWidget()
{}

void MountControlsWidget::updateMountInfoUi() {
	

    IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
    if (panTiltMount) {
        ui.labelPanAngle->setText(QString::number(panTiltMount->panAngle()));
        ui.labelTiltAngle->setText(QString::number(panTiltMount->tiltAngle()));
		ui.labelPanRange->setText(QString("%1 to %2").arg(panTiltMount->panMinAngle()).arg(panTiltMount->panMaxAngle()));
		ui.labelTiltRange->setText(QString("%1 to %2").arg(panTiltMount->tiltMinAngle()).arg(panTiltMount->tiltMaxAngle()));
    }
}

void MountControlsWidget::recenterMount()
{

}

void MountControlsWidget::onPanSliderChanged(int value)
{

}

void MountControlsWidget::onTiltSliderChanged(int value)
{

}

