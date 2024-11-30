#include "SensorWriter.h"

QImage matToQImage(const cv::Mat& mat) {
	if (mat.type() == CV_8UC3) {
		cv::Mat rgbMat;
		cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB); // Convert BGR to RGB
		return QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888).copy();
	}
	else if (mat.type() == CV_8UC4) {
		return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32).copy();
	}
	else {
		qWarning() << "Unsupported Mat format";
		return QImage();
	}
}

QVideoFrame imageToVideoFrame(const QImage& image, qint64 prevTimestamp, qint64 currTimestamp) {
	QVideoFrame frame(image);
	frame.setStartTime(prevTimestamp);
	frame.setEndTime(currTimestamp); // Set duration in milliseconds
	//frame.setStreamFrameRate(30);
	return frame;
}

SensorWriter::SensorWriter(QObject* parent)
{
	// Create output directory
	QString outputFolderName = "output";
	m_outputPath = QDir::currentPath();
	QDir _ = QDir(m_outputPath.toString());
	if (!_.exists(outputFolderName)) {
		_.mkdir(outputFolderName);
	}

	// Initialize filename
	m_filename = "test_output.avi";
	m_outputPath = m_outputPath.toString() + "/" + outputFolderName + "/" + m_filename;

	// Initialize video writer
	QVideoFrameFormat frameFormat(QSize(640, 480), QVideoFrameFormat::Format_RGBA8888);
	m_frameInput = new QVideoFrameInput(frameFormat);
	m_format.setVideoCodec(QMediaFormat::VideoCodec::MPEG4);
	m_format.setFileFormat(QMediaFormat::AVI);
	m_session.setRecorder(&m_recorder);
	m_session.setVideoFrameInput(m_frameInput);
	m_recorder.setOutputLocation(m_outputPath);
	m_recorder.setMediaFormat(m_format);
	m_recorder.setVideoFrameRate(30);
	m_recorder.setEncodingMode(QMediaRecorder::AverageBitRateEncoding);
	m_recorder.setVideoFrameRate(30);
	m_recorder.setVideoResolution(QSize(640, 480));
}

SensorWriter::~SensorWriter()
{
	if (m_isRecording) m_recorder.stop();
}

void SensorWriter::startRecording()
{
	if (!m_isRecording) {
		m_startTimestamp = QDateTime::currentMSecsSinceEpoch();
		m_previousTimestamp = m_startTimestamp;

		// Check if the output file exists
		QFile file(m_outputPath.toString());
		if (file.exists()) {
			file.remove();
		}

		file.open(QIODevice::WriteOnly);
		file.close();

		m_isRecording = true;
		m_recorder.record();
	}
}

void SensorWriter::stopRecording()
{
	if (m_isRecording) {
		m_isRecording = false;
		m_recorder.stop();

		if (m_recorder.error() != QMediaRecorder::NoError) {
			qWarning() << "Error while stopping recorder:" << m_recorder.errorString();
		}
	}
}

void SensorWriter::write(const QVariant& data, const qint64 timestamp) {
	if (!m_isRecording) return;

	if (data.canConvert<cv::Mat>()) {
		auto mat = data.value<cv::Mat>();
		qDebug() << "Frame dimensions:" << mat.cols << "x" << mat.rows;
		QImage img = matToQImage(mat);
		QVideoFrame frame = imageToVideoFrame(img, m_previousTimestamp-m_startTimestamp, timestamp-m_startTimestamp);
		if (!frame.isValid()) {
			qWarning() << "Invalid QVideoFrame created.";
		}
		if (!m_frameInput->sendVideoFrame(frame)) {
			qWarning() << "Failed to send video frame to recorder.";
		}
	}
	
	m_previousTimestamp = timestamp;

	emit writeFinished(timestamp);

}