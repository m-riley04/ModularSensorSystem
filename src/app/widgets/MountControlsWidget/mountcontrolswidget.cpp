#include "mountcontrolswidget.h"
#include <interfaces/capability/mounts/ipantiltmount.hpp>

MountControlsWidget::MountControlsWidget(Mount* mount, MainController& mc, QWidget *parent)
	: QWidget(parent), m_mainController(mc), m_mount(mount)
{
	ui.setupUi(this);

	if (!m_mount) {
		LoggingController::critical("MountControlsWidget initialized with null mount pointer.");
		return;
	}

	// Initialize UI before connections
	updateUi();
	ui.labelMountName->setText(QString::fromStdString(m_mount->displayName()));

	// Connect mount notify signal to UI update
	connect(m_mount, &Mount::dataUpdated, this, [this]() {
		updateUi();
		});
	connect(m_mount, &QObject::destroyed, this, [this]() {
		m_mount = nullptr;
		});

	connect(ui.sliderPan, &QSlider::valueChanged, this, &MountControlsWidget::onPanSliderChanged);
	connect(ui.sliderTilt, &QSlider::valueChanged, this, &MountControlsWidget::onTiltSliderChanged);
	connect(ui.buttonRecenter, &QPushButton::clicked, this, &MountControlsWidget::onRecenterClicked);
	connect(ui.buttonSetInitialAngles, &QPushButton::clicked, this, &MountControlsWidget::onSetInitialAnglesClicked);
	connect(ui.buttonRefresh, &QPushButton::clicked, this, &MountControlsWidget::onRefreshInfoClicked);
}

MountControlsWidget::~MountControlsWidget()
{
	// Disconnect to prevent accessing deleted mount
	if (m_mount) {
		disconnect(m_mount, nullptr, this, nullptr);
		m_mount = nullptr;
	}
}

void MountControlsWidget::updateUi()
{
    IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);

    if (!panTiltMount) {
		LoggingController::warning("Mount does not support IPanTiltMount interface.");
        ui.labelPanAngle->setText(QString("N/A"));
        ui.labelTiltAngle->setText(QString("N/A"));
		ui.labelPanRange->setText(QString("N/A"));
		ui.labelTiltRange->setText(QString("N/A"));

		// Disable controls
		ui.groupSliders->setEnabled(false);
		ui.frameButtons->setEnabled(false);

		// Set slider positions to 0
		ui.sliderPan->setValue(0);
		ui.sliderTilt->setValue(0);

		// Set slider ranges to 0-100
		ui.sliderPan->setMinimum(0);
		ui.sliderPan->setMaximum(180);
		ui.sliderTilt->setMinimum(0);

		return;
    }
	
    ui.labelPanAngle->setText(QString::number(panTiltMount->panAngle()));
    ui.labelTiltAngle->setText(QString::number(panTiltMount->tiltAngle()));
	ui.labelPanRange->setText(QString("%1 to %2").arg(panTiltMount->panMinAngle()).arg(panTiltMount->panMaxAngle()));
	ui.labelTiltRange->setText(QString("%1 to %2").arg(panTiltMount->tiltMinAngle()).arg(panTiltMount->tiltMaxAngle()));

	ui.groupSliders->setEnabled(true);
	ui.frameButtons->setEnabled(true);

	// Set slider positions
	ui.sliderPan->blockSignals(true);
	ui.sliderPan->setValue(static_cast<int>(panTiltMount->panAngle()));
	ui.sliderPan->blockSignals(false);

	ui.sliderTilt->blockSignals(true);
	ui.sliderTilt->setValue(static_cast<int>(panTiltMount->tiltAngle()));
	ui.sliderTilt->blockSignals(false);

	// Set slider ranges
	ui.sliderPan->setMinimum(static_cast<int>(panTiltMount->panMinAngle()));
	ui.sliderPan->setMaximum(static_cast<int>(panTiltMount->panMaxAngle()));
	ui.sliderTilt->setMinimum(static_cast<int>(panTiltMount->tiltMinAngle()));
	ui.sliderTilt->setMaximum(static_cast<int>(panTiltMount->tiltMaxAngle()));

}

void MountControlsWidget::onRefreshInfoClicked()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;
	if (!panTiltMount->refreshInfo()) {
		LoggingController::warning("Failed to refresh mount info.");
	}
	updateUi();
}

void MountControlsWidget::onSetInitialAnglesClicked()
{
    IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
    if (!panTiltMount) return;

    double panAngle = static_cast<double>(ui.sliderPan->value());
    double tiltAngle = static_cast<double>(ui.sliderTilt->value());
    if (!panTiltMount->moveTo(panAngle, tiltAngle)) {
        LoggingController::warning("Failed to move to the specified angles.");
    }
    updateUi();
}

void MountControlsWidget::onRecenterClicked()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	if (!panTiltMount->recenter()) {
		LoggingController::warning("Failed to recenter the mount.");
	}

	updateUi();
}

void MountControlsWidget::onPanSliderChanged(int value)
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	if (!panTiltMount->moveTo(static_cast<double>(value), panTiltMount->tiltAngle())) {
		LoggingController::warning("Failed to move pan to the specified value.");
	}
	updateUi();
}

void MountControlsWidget::onTiltSliderChanged(int value)
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	if (!panTiltMount->moveTo(panTiltMount->panAngle(), static_cast<double>(value))) {
		LoggingController::warning("Failed to move tilt to the specified value.");
	}
	updateUi();
}

