#include "logging/logwriterworker.hpp"
#include <QTextStream>
#include <QDateTime>
#include <QMutexLocker>
#include <QTimer>

LogWriterWorker::LogWriterWorker(QObject* parent)
	: QObject(parent)
{
}

LogWriterWorker::~LogWriterWorker()
{
	// Ensure cleanup if not already stopped
	if (m_running.load()) {
		stop(true);
	}
}

void LogWriterWorker::enqueueMessage(const QString& message)
{
	QMutexLocker locker(&m_queueMutex);
	m_queue.enqueue(message);
}

bool LogWriterWorker::isRunning() const
{
	return m_running.load();
}

int LogWriterWorker::queueSize() const
{
	QMutexLocker locker(&m_queueMutex);
	return m_queue.size();
}

void LogWriterWorker::start(const QString& logFilePath)
{
	if (m_running.load()) {
		return; // Already running
	}

	if (!openFile(logFilePath)) {
		emit errorOccurred(QString("Failed to open log file: %1").arg(logFilePath));
		return;
	}

	m_running.store(true);

	// Write opening message
	writeMessage(QString("----- Log file opened at %1 -----")
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")));

	// Create and start timer to process queue periodically
	// Timer runs on this thread's event loop, allowing stop() to be received
	m_processTimer = new QTimer(this);
	connect(m_processTimer, &QTimer::timeout, this, &LogWriterWorker::processQueue);
	m_processTimer->start(10); // Process queue every 10ms
}

void LogWriterWorker::stop(bool flush)
{
	if (!m_running.load()) {
		return; // Not running
	}

	// Stop the timer first
	if (m_processTimer) {
		m_processTimer->stop();
		delete m_processTimer;
		m_processTimer = nullptr;
	}

	// Flush remaining messages if requested
	if (flush) {
		flushQueue();
	}

	// Write closing message
	writeMessage(QString("----- Log file closed at %1 -----")
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")));

	closeFile();
	m_running.store(false);

	emit finished();
}

void LogWriterWorker::processQueue()
{
	// Process a batch of messages per timer tick to avoid blocking too long
	const int maxMessagesPerTick = 50;
	int processed = 0;

	while (processed < maxMessagesPerTick) {
		QString message;
		{
			QMutexLocker locker(&m_queueMutex);
			if (m_queue.isEmpty()) {
				break;
			}
			message = m_queue.dequeue();
		}
		writeMessage(message);
		++processed;
	}
}

void LogWriterWorker::flushQueue()
{
	// Write all remaining messages
	QMutexLocker locker(&m_queueMutex);
	while (!m_queue.isEmpty()) {
		QString msg = m_queue.dequeue();
		locker.unlock();
		writeMessage(msg);
		locker.relock();
	}
}

void LogWriterWorker::writeMessage(const QString& message)
{
	if (!m_logFile || !m_logFile->isOpen()) {
		return;
	}

	*m_logStream << message << Qt::endl;
	m_logStream->flush();
}

bool LogWriterWorker::openFile(const QString& filePath)
{
	if (m_logFile) {
		closeFile();
	}

	m_logFile = new QFile(filePath, this);
	
	if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
		delete m_logFile;
		m_logFile = nullptr;
		return false;
	}	

	m_logStream = new QTextStream(m_logFile);

	return true;
}

void LogWriterWorker::closeFile()
{
	if (m_logFile) {
		if (m_logFile->isOpen()) {
			m_logFile->close();
		}
		delete m_logFile;
		m_logFile = nullptr;
	}
	if (m_logStream) {
		m_logStream->flush();
		delete m_logStream;
		m_logStream = nullptr;
	}
}