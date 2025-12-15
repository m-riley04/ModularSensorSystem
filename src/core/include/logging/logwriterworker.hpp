#pragma once

#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QQueue>
#include <QMutex>
#include <QTimer>
#include <atomic>
#include "core_export.hpp"

/**
 * @brief Worker class that handles asynchronous log file writing on a dedicated thread.
 * 
 * Messages are enqueued via enqueueMessage() (thread-safe, non-blocking) and written
 * to disk by the worker thread. Call start() to begin processing and stop() to 
 * gracefully shut down (flushes remaining messages before stopping).
 */
class MSS_CORE_API LogWriterWorker : public QObject
{
	Q_OBJECT

public:
	explicit LogWriterWorker(QObject* parent = nullptr);
	~LogWriterWorker() override;

	/**
	 * @brief Enqueue a message to be written asynchronously.
	 * Thread-safe and non-blocking.
	 * @param message The formatted log message to write.
	 */
	void enqueueMessage(const QString& message);

	/**
	 * @brief Check if the worker is currently running.
	 * @return true if the worker thread is processing messages.
	 */
	bool isRunning() const;

	/**
	 * @brief Get the current queue size (approximate, for diagnostics).
	 * @return Number of messages waiting to be written.
	 */
	int queueSize() const;

public slots:
	/**
	 * @brief Open the log file and start processing the queue.
	 * Should be called after moving worker to its thread.
	 * @param logFilePath Full path to the log file.
	 */
	void start(const QString& logFilePath);

	/**
	 * @brief Stop processing and close the log file.
	 * Flushes all remaining messages before stopping.
	 * @param flush If true, write all queued messages before stopping.
	 */
	void stop(bool flush = true);

signals:
	/**
	 * @brief Emitted when the worker has finished and the thread can be quit.
	 */
	void finished();

	/**
	 * @brief Emitted when an error occurs during file operations.
	 * @param error Description of the error.
	 */
	void errorOccurred(const QString& error);

private slots:
	void processQueue();

private:
	void writeMessage(const QString& message);
	void flushQueue();
	bool openFile(const QString& filePath);
	void closeFile();

	QFile* m_logFile = nullptr;
	QTextStream* m_logStream = nullptr;
	QQueue<QString> m_queue;
	mutable QMutex m_queueMutex;
	QTimer* m_processTimer = nullptr;
	std::atomic<bool> m_running{ false };
};