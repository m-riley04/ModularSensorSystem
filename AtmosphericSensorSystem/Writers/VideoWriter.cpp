#include "VideoWriter.h"

VideoWriter::VideoWriter(const std::string& filename, int fourcc, double fps, cv::Size frameSize)
	: SensorWriter(), filename(filename), fourcc(fourcc), fps(fps), frameSize(frameSize)
{
    // Open the VideoWriter with the provided parameters
    writer.open(filename, fourcc, fps, frameSize, true);

    if (!writer.isOpened()) {
        qWarning() << "Failed to open video file for writing:" << QString::fromStdString(filename);
    }
    else {
        qDebug() << "VideoRecorder initialized for file:" << QString::fromStdString(filename);
    }
}

VideoWriter::~VideoWriter()
{
    // Release the VideoWriter resource
    if (writer.isOpened()) {
        writer.release();
        qDebug() << "VideoRecorder released the video file:" << QString::fromStdString(filename);
    }
}

void VideoWriter::record(const QVariant& data) {
    if (!writer.isOpened()) {
        qWarning() << "VideoWriter is not opened. Cannot record data.";
        return;
    }

    // Convert QVariant to cv::Mat
    if (data.canConvert<cv::Mat>()) {
        cv::Mat frame = data.value<cv::Mat>();

        // Optionally, check if frame size matches expected frame size
        if (frame.size() != frameSize) {
            qWarning() << "Frame size does not match. Expected:"
                << frameSize.width << "x" << frameSize.height
                << "Got:" << frame.cols << "x" << frame.rows;
            // You might want to resize or handle this case differently
            return;
        }

        // Write the frame to the video file
        writer.write(frame);
    }
    else {
        qWarning() << "Invalid data type. Expected cv::Mat.";
    }
}

bool VideoWriter::isOpened() const {
    return writer.isOpened();
}