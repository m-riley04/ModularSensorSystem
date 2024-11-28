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

SensorWriter::SensorWriter(QObject* parent)
{
	// Create output directory
	QString outputFolderName = "output";
	outputPath = QDir::currentPath();
	QDir _ = QDir(outputPath.toString());
	if (!_.exists(outputFolderName)) {
		_.mkdir(outputFolderName);
	}

	// Initialize filename
	filename = "test_output.mp4";
	outputPath = outputPath.toString() + "/" + outputFolderName + "/" + filename;

	// Initialize video writer
	QVideoFrameFormat frameFormat(QSize(640, 480), QVideoFrameFormat::Format_RGBA8888);
	frameInput = new QVideoFrameInput(frameFormat);
	format.setVideoCodec(QMediaFormat::VideoCodec::MPEG4);
	format.setFileFormat(QMediaFormat::MPEG4);
	session.setRecorder(&recorder);
	session.setVideoFrameInput(frameInput);
	recorder.setOutputLocation(outputPath);
	recorder.setMediaFormat(format);
	recorder.setVideoResolution(QSize(640, 480));
}

SensorWriter::~SensorWriter()
{
	if (isRecording) recorder.stop();
}

void SensorWriter::startRecording()
{
	if (!isRecording) {
		// Check if the output file exists
		QFile file(outputPath.toString());
		if (file.exists()) {
			file.remove();
		}

		file.open(QIODevice::WriteOnly);
		file.close();

		isRecording = true;
		recorder.record();
	}
}

void SensorWriter::stopRecording()
{
	if (isRecording) {
		isRecording = false;
		recorder.stop();

		if (recorder.error() != QMediaRecorder::NoError) {
			qWarning() << "Error while stopping recorder:" << recorder.errorString();
		}
	}
}

void SensorWriter::write(const QVariant& data, const qint64 timestamp) {
	if (!isRecording) return;

	if (data.canConvert<cv::Mat>()) {
		auto mat = data.value<cv::Mat>();
		qDebug() << "Frame dimensions:" << mat.cols << "x" << mat.rows;
		QImage img = matToQImage(mat);
		QVideoFrame frame(img);
		if (!frame.isValid()) {
			qWarning() << "Invalid QVideoFrame created.";
		}
		if (!frameInput->sendVideoFrame(frame)) {
			qWarning() << "Failed to send video frame to recorder.";
		}
	}
	emit writeFinished(timestamp);

}