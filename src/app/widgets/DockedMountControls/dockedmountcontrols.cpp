#include "dockedmountcontrols.h"
#include <interfaces/capability/mounts/ipantiltmount.hpp>
#include <controllers/loggingcontroller.hpp>

DockedMountControls::DockedMountControls(QWidget* parent)
	: QDockWidget(parent), m_mount(nullptr)
{
	ui.setupUi(this);

	// Set the contents widget for the dock
	this->setWidget(ui.contents);

	connect(ui.sliderPan, &QSlider::valueChanged, this, &DockedMountControls::onPanSliderChanged);
	connect(ui.sliderTilt, &QSlider::valueChanged, this, &DockedMountControls::onTiltSliderChanged);
	connect(ui.buttonRecenter, &QPushButton::clicked, this, &DockedMountControls::onRecenterClicked);
	connect(ui.buttonSetInitialAngles, &QPushButton::clicked, this, &DockedMountControls::onSetInitialAnglesClicked);
	connect(ui.buttonRefresh, &QPushButton::clicked, this, &DockedMountControls::onRefreshInfoClicked);

	updateUi();
}

DockedMountControls::~DockedMountControls()
{
	// Disconnect to prevent accessing deleted mount
	if (m_mount) {
		disconnect(m_mount, nullptr, this, nullptr);
		m_mount = nullptr;
	}
}

void DockedMountControls::setMount(Mount* mount)
{
	if (m_mount == mount) return;
	if (m_mount) {
		// Disconnect from previous mount
		disconnect(m_mount, nullptr, this, nullptr);
	}

	m_mount = mount;

	if (m_mount) {
		// Connect mount notify signal to UI update
		connect(m_mount, &QObject::destroyed, this, [this]() {
			m_mount = nullptr;
			updateUi();
			});
		connect(m_mount, &Mount::dataUpdated, this, [this]() {
			updateUi();
			});
	}

	updateUi();
}

void DockedMountControls::updateUi()
{

	if (!m_mount) {
		LoggingController::warning("Mount is null or does not support IPanTiltMount interface.");
		ui.labelMountName->setText("N/A");
		ui.stack->setCurrentWidget(ui.pageNone);
		return;
	}
	ui.labelMountName->setText(QString::fromStdString(m_mount->displayName()));
	
	// Select appropriate page
	if (dynamic_cast<IPanTiltMount*>(m_mount)) {
		ui.stack->setCurrentWidget(ui.pagePanTilt);
		updatePanTiltUi();
		return;
	}

	// Select static page stack
	ui.stack->setCurrentWidget(ui.pageStatic);

}

void DockedMountControls::updatePanTiltUi()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);

	if (!m_mount || !panTiltMount) {
		LoggingController::warning("Could not update pan-tilt UI: mount is null or does not support IPanTiltMount interface.");
		ui.labelPanAngle->setText("N/A");
		ui.labelTiltAngle->setText("N/A");
		ui.labelPanRange->setText("N/A");
		ui.labelTiltRange->setText("N/A");

		// Disable pan tilt page
		ui.pagePanTilt->setEnabled(false);

		// Set slider positions to 0
		ui.sliderPan->setValue(0);
		ui.sliderTilt->setValue(0);

		// Set slider ranges to 0-100
		ui.sliderPan->setMinimum(0);
		ui.sliderPan->setMaximum(0);
		ui.sliderTilt->setMinimum(0);
		ui.sliderTilt->setMaximum(0);

		return;
	}

	Pose pose = panTiltMount->pose();
	ui.labelPanAngle->setText(QString::number(pose.yaw));
	ui.labelTiltAngle->setText(QString::number(pose.pitch));
	ui.labelPanRange->setText(QString("%1 to %2").arg(pose.bounds.yaw.min).arg(pose.bounds.yaw.max));
	ui.labelTiltRange->setText(QString("%1 to %2").arg(pose.bounds.pitch.min).arg(pose.bounds.pitch.max));

	// Set slider positions
	ui.sliderPan->blockSignals(true);
	ui.sliderPan->setValue(static_cast<int>(pose.yaw));
	ui.sliderPan->blockSignals(false);

	ui.sliderTilt->blockSignals(true);
	ui.sliderTilt->setValue(static_cast<int>(pose.pitch));
	ui.sliderTilt->blockSignals(false);

	// Set slider ranges
	ui.sliderPan->setMinimum(static_cast<int>(pose.bounds.yaw.min));
	ui.sliderPan->setMaximum(static_cast<int>(pose.bounds.yaw.max));
	ui.sliderTilt->setMinimum(static_cast<int>(pose.bounds.pitch.min));
	ui.sliderTilt->setMaximum(static_cast<int>(pose.bounds.pitch.max));

	// Enable pan tilt page
	ui.pagePanTilt->setEnabled(true);
}

void DockedMountControls::onRefreshInfoClicked()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;
	if (!panTiltMount->refreshInfo()) {
		LoggingController::warning("Failed to refresh mount info.");
	}
}

void DockedMountControls::onSetInitialAnglesClicked()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	double panAngle = static_cast<double>(ui.sliderPan->value());
	double tiltAngle = static_cast<double>(ui.sliderTilt->value());
	if (!panTiltMount->moveTo(panAngle, tiltAngle)) {
		LoggingController::warning("Failed to move to the specified angles.");
	}
}

void DockedMountControls::onRecenterClicked()
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	if (!panTiltMount->recenter()) {
		LoggingController::warning("Failed to recenter the mount.");
	}
}

void DockedMountControls::onPanSliderChanged(int value)
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	Pose pose = panTiltMount->pose();
	if (!panTiltMount->moveTo(static_cast<double>(value), pose.pitch)) {
		LoggingController::warning("Failed to move pan to the specified value.");
	}
}

void DockedMountControls::onTiltSliderChanged(int value)
{
	IPanTiltMount* panTiltMount = dynamic_cast<IPanTiltMount*>(m_mount);
	if (!panTiltMount) return;

	Pose pose = panTiltMount->pose();
	if (!panTiltMount->moveTo(pose.yaw, static_cast<double>(value))) {
		LoggingController::warning("Failed to move tilt to the specified value.");
	}
}

