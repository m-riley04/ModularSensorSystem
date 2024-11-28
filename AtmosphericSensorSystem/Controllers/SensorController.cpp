#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize camera and thread
	camera = new Camera();
	worker = new QThread(this);
	addSensor(camera);
	camera->moveToThread(worker);

	// Start thread
	connect(worker, &QThread::started, camera, &Camera::start);
	connect(camera, &Camera::dataReady, qobject_cast<MainWindow*>(parent->parent()), &MainWindow::displayFrame);
	connect(this, &SensorController::stopSensors, camera, &Camera::stop);
	connect(worker, &QThread::finished, camera, &QObject::deleteLater);
	worker->start();
}

SensorController::~SensorController()
{
	// Stop the thread safely
	worker->quit();
	worker->wait();
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
