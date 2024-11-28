#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize components
	camera = new Camera();
	addSensor(camera);

	synchronizer = new SensorSynchronizer();
	frameProcessor = new FrameProcessor();
	writer = new SensorWriter();
	
	// Initialize threads
	cameraThread = new QThread();
	processorThread = new QThread();
	writerThread = new QThread();

	camera->moveToThread(cameraThread);
	frameProcessor->moveToThread(processorThread);
	writer->moveToThread(writerThread);

	// Connect pipeline
	connect(camera, &Camera::dataReady, frameProcessor, &FrameProcessor::processFrame);
	connect(frameProcessor, &FrameProcessor::frameProcessed, synchronizer, &SensorSynchronizer::addSensorData);
	connect(synchronizer, &SensorSynchronizer::synchronizedDataReady, writer, &SensorWriter::write);
	connect(synchronizer, &SensorSynchronizer::synchronizedDataReady, qobject_cast<MainWindow*>(parent->parent()), &MainWindow::displayFrame);

	// Connect other signals/slots
	MainWindow* mainWindow = qobject_cast<MainWindow*>(parent->parent());
	connect(cameraThread, &QThread::started, camera, &Camera::start);
	connect(this, &SensorController::stopSensors, camera, &Camera::stop);
	connect(mainWindow, &MainWindow::clicked_record, writer, &SensorWriter::startRecording);
	connect(mainWindow, &MainWindow::clicked_stop, writer, &SensorWriter::stopRecording);
	
	// Start threads
	cameraThread->start();
	processorThread->start();
	writerThread->start();
}

SensorController::~SensorController()
{
	// Stop the threads safely
	cameraThread->quit();
	cameraThread->wait();

	processorThread->quit();
	processorThread->wait();

	writerThread->quit();
	writerThread->wait();
}

QList<Sensor*> SensorController::sensors() const
{
	return _sensors;
}

void SensorController::addSensor(Sensor* sensor)
{
	_sensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorController::removeSensor(Sensor* sensor)
{
	_sensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorController::clearSensors()
{
	_sensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (Sensor* sensor : _sensors) {
		QMetaObject::invokeMethod(sensor, "start", Qt::QueuedConnection);
	}
}

void SensorController::stopSensors()
{
	for (Sensor* sensor : _sensors) {
		QMetaObject::invokeMethod(sensor, "stop", Qt::QueuedConnection);
	}
}
