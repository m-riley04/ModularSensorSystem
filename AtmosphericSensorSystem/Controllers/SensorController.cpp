#include "SensorController.h"

SensorController::SensorController(QObject *parent, QLabel* label)
	: QObject(parent), label(label)
{
	// Initialize camera and thread
	camera = new Camera();
	worker = new QThread(this);
	addSensor(camera);
	camera->moveToThread(worker);

	// Start thread
	worker->start();
	connect(worker, &QThread::finished, camera, &QObject::deleteLater);

	// Initialize the timer to capture frames
	connect(camera, &Camera::dataReady, this, [this](QVariant data) {
		if (data.canConvert<cv::Mat>()) {
			// Use QMetaObject::invokeMethod to ensure 'display' runs in the main thread
			QMetaObject::invokeMethod(this, "display", Qt::QueuedConnection, Q_ARG(cv::Mat, data.value<cv::Mat>()));
		}
	});
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

void SensorController::display(const cv::Mat& frame)
{
	QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
	label->setPixmap(QPixmap::fromImage(image).scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}
