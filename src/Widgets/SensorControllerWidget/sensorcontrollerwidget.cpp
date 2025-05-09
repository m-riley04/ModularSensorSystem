#include "sensorcontrollerwidget.h"

SensorControllerWidget::SensorControllerWidget(QWidget *parent, SensorController *sensorController)
	: QWidget(parent), pController(sensorController)
{
	ui.setupUi(this);

    initWidgets();
    initSignals();
}

SensorControllerWidget::~SensorControllerWidget()
{
    mSinkWidgets.clear();
}

void SensorControllerWidget::initWidgets()
{
    // Set the tab to be empty
    ui.tabSensors->clear();
}

void SensorControllerWidget::initSignals() {

    // Camera View
    connect(ui.buttonAddSensor, &QPushButton::clicked, this, &SensorControllerWidget::openAddCameraDialog);
    connect(ui.buttonRemoveSensor, &QPushButton::clicked, this, &SensorControllerWidget::removeCamera);
    connect(pController, &SensorController::cameraAdded, this, &SensorControllerWidget::addVideoWidget);

    // Controls
    connect(ui.checkboxControls, &QCheckBox::toggled, this, &SensorControllerWidget::controlsChanged);

    // Sensor tab changed logic
    connect(ui.tabSensors, &QTabWidget::currentChanged, [this](int index) {
        if (index == -1 || !(index < mSinkWidgets.size())) return;

        // Get the camera from the video widgets
        Camera* cam = pController->findCamera(mSinkWidgets.at(index));

        // Emit the camera changed signal
        emit cameraChanged(cam);
        });
}

void SensorControllerWidget::addVideoWidget(Camera* camera)
{
    auto sinkWidget = new CustomSinkWidget(this);
    mSinkWidgets.push_back(sinkWidget);
    QString name = camera->camera()->cameraDevice().description();
    camera->setVideoOutput(sinkWidget); // Set output BEFORE adding tab
    ui.tabSensors->addTab(sinkWidget, name);

    emit videoWidgetAdded(camera);
}

void SensorControllerWidget::openAddCameraDialog()
{
    AddCameraDialog* addDialog = new AddCameraDialog(this, pController->videoDevices());
    connect(addDialog, &AddCameraDialog::deviceSelected, pController, &SensorController::addCamera);
    addDialog->show();
}

void SensorControllerWidget::removeCamera()
{
    const QString title = "Delete";
    const QString text = "Are you sure you want to remove this camera?";
    QMessageBox::StandardButton reply = QMessageBox::question(this, title, text, QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

    // Check output
    if (reply == QMessageBox::Yes)
    {
        // Find the selected tab
        int currentTabIndex = ui.tabSensors->currentIndex();
        if (currentTabIndex == -1 || !(currentTabIndex < mSinkWidgets.size()))
        {
            QMessageBox::warning(this, "Error", "The selected camera does not exist and cannot be deleted.", QMessageBox::StandardButton::Ok);
            return;
        }

        // Find camera and video widget
        auto& videoWidget = mSinkWidgets.at(currentTabIndex);
        Camera* cam = pController->findCamera(videoWidget);

        // Remove video widget from UI
        videoWidget->deleteLater();

        // Remove camera and video widget
        pController->removeCamera(cam);
        mSinkWidgets.erase(mSinkWidgets.begin() + currentTabIndex);
    }
}