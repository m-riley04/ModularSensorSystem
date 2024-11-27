#include "MainWindow.h"
#include "Widgets/SensorListItem.h"
#include <QThread>
#include <QListWidget>

QString getDeviceClassString(uint8_t classCode) {
    switch (classCode) {
    case LIBUSB_CLASS_PER_INTERFACE: return "Device";
    case LIBUSB_CLASS_AUDIO: return "Audio";
    case LIBUSB_CLASS_COMM: return "Communications";
    case LIBUSB_CLASS_HID: return "Human Interface Device";
    case LIBUSB_CLASS_PHYSICAL: return "Physical";
    case LIBUSB_CLASS_PRINTER: return "Printer";
    case LIBUSB_CLASS_IMAGE: return "Image";
    case LIBUSB_CLASS_MASS_STORAGE: return "Mass Storage";
    case LIBUSB_CLASS_HUB: return "Hub";
    case LIBUSB_CLASS_DATA: return "Data";
    case LIBUSB_CLASS_SMART_CARD: return "Smart Card";
    case LIBUSB_CLASS_CONTENT_SECURITY: return "Content Security";
    case LIBUSB_CLASS_VIDEO: return "Video";
    case LIBUSB_CLASS_PERSONAL_HEALTHCARE: return "Personal Healthcare";
    case LIBUSB_CLASS_DIAGNOSTIC_DEVICE: return "Diagnostic Device";
    case LIBUSB_CLASS_WIRELESS: return "Wireless";
    case LIBUSB_CLASS_APPLICATION: return "Application";
    case LIBUSB_CLASS_VENDOR_SPEC: return "Vendor Specific";
    default: return "Unknown";
    }
}

void populateSensorsList(QListWidget* sensorsList) {

    libusb_context* context = nullptr;
    libusb_device** devices = nullptr;
    ssize_t deviceCount;

    // Initialize libusb context
    if (libusb_init(&context) < 0) {
        std::cerr << "Failed to initialize libusb" << std::endl;
        return;
    }

    // Get list of USB devices
    deviceCount = libusb_get_device_list(context, &devices);
    if (deviceCount < 0) {
        std::cerr << "Failed to get USB device list" << std::endl;
        libusb_exit(context);
        return;
    }

    // Iterate over devices
    for (ssize_t i = 0; i < deviceCount; ++i) {
        libusb_device* device = devices[i];
        libusb_device_descriptor descriptor;
		QString portName, vendorId, productId, manufacturerName, productName, className;

        // Get device descriptor
        if (libusb_get_device_descriptor(device, &descriptor) == 0) {
            portName = "COM" + QString::number(i);
			vendorId = QString::number(descriptor.idVendor, 16);
			productId = QString::number(descriptor.idProduct, 16);

            // Get Class string
            className = getDeviceClassString(descriptor.bDeviceSubClass);


            // Open device to fetch further details (optional)
            libusb_device_handle* handle;
            if (libusb_open(device, &handle) == 0) {
                unsigned char buffer[256];

                // Get Manufacturer string
                if (descriptor.iManufacturer && libusb_get_string_descriptor_ascii(handle, descriptor.iManufacturer, buffer, sizeof(buffer)) > 0) {
                    manufacturerName = QString::fromUtf8(reinterpret_cast<const char*>(buffer));
                }

                // Get Product string
                if (descriptor.iProduct && libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, buffer, sizeof(buffer)) > 0) {
                    productName = QString::fromUtf8(reinterpret_cast<const char*>(buffer));
                }

                // Close device handle
                libusb_close(handle);
            }
        }

		// Add the sensor to the list
        QListWidgetItem* item = new QListWidgetItem(sensorsList);
        SensorListItem* sensorWidget = new SensorListItem(portName, className, manufacturerName, productName);
        item->setSizeHint(sensorWidget->sizeHint());
        sensorsList->setItemWidget(item, sensorWidget);
    }

    // Clean up
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Initialize
    initSensors();
    initSignals();
    initWidgets();

}

MainWindow::~MainWindow()
{
    sensorController->stopSensors();
    delete sensorController;
}

void MainWindow::initWidgets() {

}

void MainWindow::initSensors() {
    sensorController = new SensorController(this, ui.video);
}

void MainWindow::initSignals() {
    // Recording Controls
    connect(ui.buttonRecord, &QPushButton::clicked, this, &MainWindow::record_clicked); /// TODO: Change this wording to be more like "toggleRecording"
    connect(ui.buttonOpenOutputDirectory, &QPushButton::clicked, this, &MainWindow::openOutputDirectory);
    connect(ui.buttonSetOutputDirectory, &QPushButton::clicked, this, &MainWindow::setOutputDirectory);
    connect(ui.outputPrefixLineEdit, &QLineEdit::textChanged, this, &MainWindow::setOutputPrefix);

    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, sensorController, &SensorController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, sensorController, &SensorController::startSensors);
    

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
    connect(ui.actionCameraProperties, &QAction::triggered, this, &MainWindow::openCameraProperties);
}

void MainWindow::record_clicked() {
	if (isRecording) {
        //disconnect(camera, &Camera::dataReady, videoWriter, &VideoWriter::record);

		// TODO: Add cleanup code for videoWriter
	}
	else {
		//QString filename = QFileDialog::getSaveFileName(this, "Save video file", camera->outputDirectory().toString(), "Video files (*.avi *.mp4)");
		//if (filename.isEmpty()) {
		//	return;
		//}

  //      // Get the frame rate
  //      double fps = camera->fps();
  //      cv::Size frameSize(camera->frameWidth(), camera->frameHeight());
  //      int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

		//videoWriter = new VideoWriter(filename.toStdString(), fourcc, fps, frameSize);
  //      connect(camera, &Camera::dataReady, videoWriter, &VideoWriter::record);
	}

    isRecording = !isRecording;
}

void MainWindow::displayFrame(const cv::Mat& frame) {
    // Convert the frame to QImage for displaying in QLabel
    QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::openOutputDirectory() {
    //QDesktopServices::openUrl(camera->outputDirectory());
}

void MainWindow::setOutputDirectory() {
    QUrl temp = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (!temp.isEmpty()) {
        // Update UI
		ui.outputPrefixLineEdit->setText(temp.toString());

        // Update camera
        //camera->setOutputDirectory(temp);
    }
}

void MainWindow::setOutputPrefix()
{
	//camera->setOutputPrefix(ui.outputPrefixLineEdit->text());
}

void MainWindow::openCameraProperties() {
    //CameraPropertiesWindow *w = new CameraPropertiesWindow(this, camera);
    //w->setAttribute(Qt::WA_DeleteOnClose);
    //w->show();
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}