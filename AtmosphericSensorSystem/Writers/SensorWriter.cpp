#include "SensorWriter.h"

QImage matToQImage(const cv::Mat& mat) {
	if (mat.type() == CV_8UC3) {
		return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
	}
	else if (mat.type() == CV_8UC4) {
		return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
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
	filename = "test_output.avi";
	outputPath = outputPath.toString() + "/" + outputFolderName + "/" + filename;

	// Initialize video writer
	QVideoFrame frame;
	QMediaCaptureSession session;
	videoWriter = new QMediaRecorder();
	session.setRecorder(videoWriter);
	videoWriter->stop();
	
	QVideoFrameInput input;

	input.sendVideoFrame()
}

SensorWriter::~SensorWriter()
{
	videoWriter->stop();
}

void SensorWriter::write(const QVariant& data, const qint64 timestamp) {
	

	if (data.canConvert<cv::Mat>()) {
		QVideoFrame frame(matToQImage(data.value<cv::Mat>()));
		input.sendVideoframe(frame);
	}
	
	
	emit writeFinished(timestamp);
}