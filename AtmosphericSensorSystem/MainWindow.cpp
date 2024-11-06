#include "MainWindow.h"

void listUSBDevices() {
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

        // Get device descriptor
        if (libusb_get_device_descriptor(device, &descriptor) == 0) {
            qDebug() << "Device " << i + 1 << ":";
            qDebug() << "  Vendor ID  : " << std::hex << descriptor.idVendor;
            qDebug() << "  Product ID : " << std::hex << descriptor.idProduct;

            // Open device to fetch further details (optional)
            libusb_device_handle* handle;
            if (libusb_open(device, &handle) == 0) {
                unsigned char buffer[256];

                // Get Manufacturer string
                if (descriptor.iManufacturer && libusb_get_string_descriptor_ascii(handle, descriptor.iManufacturer, buffer, sizeof(buffer)) > 0) {
                    qDebug() << "  Manufacturer: " << buffer;
                }

                // Get Product string
                if (descriptor.iProduct && libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, buffer, sizeof(buffer)) > 0) {
                    qDebug() << "  Product     : " << buffer;
                }

                // Close device handle
                libusb_close(handle);
            }
        }
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
{}

void MainWindow::initWidgets() {

}

void MainWindow::initSensors() {
    listUSBDevices();

    sensorController = new SensorController(this);

    // Create camera
    camera = new Camera(this);
    
    // Initialize the timer to capture frames
    connect(camera, &Camera::dataReady, this, [this](QVariant data) {
		if (data.canConvert<cv::Mat>()) {
			displayFrame(data.value<cv::Mat>());
		}
    });
}

void MainWindow::initSignals() {
    // Buttons
    //connect(ui.buttonRecord, &QPushButton::clicked, camera, &Camera::startRecording); /// TODO: Change this wording to be more like "toggleRecording"
    connect(ui.buttonOpenOutputDirectory, &QPushButton::clicked, this, &MainWindow::openOutputDirectory);
    connect(ui.buttonSetOutputDirectory, &QPushButton::clicked, this, &MainWindow::setOutputDirectory);
    connect(ui.buttonStop, &QPushButton::clicked, camera, &Camera::stop);
    connect(ui.buttonStart, &QPushButton::clicked, camera, &Camera::start);
    connect(ui.buttonRestart, &QPushButton::clicked, camera, &Camera::restart);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
    connect(ui.actionCameraProperties, &QAction::triggered, this, &MainWindow::openCameraProperties);
}



void MainWindow::detectSensors() {
    //listUSBDevices();
}

void MainWindow::displayFrame(const cv::Mat& frame) {
    // Convert the frame to QImage for displaying in QLabel
    QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::openOutputDirectory() {
    QDesktopServices::openUrl(camera->outputDirectory());
}

void MainWindow::setOutputDirectory() {
    QUrl temp = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (!temp.isEmpty()) {
        camera->setOutputDirectory(temp);
    }
}

void MainWindow::openCameraProperties() {
    CameraPropertiesWindow *w = new CameraPropertiesWindow(this, camera);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}