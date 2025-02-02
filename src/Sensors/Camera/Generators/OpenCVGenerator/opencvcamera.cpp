#include "opencvcamera.h"

OpenCVGenerator::OpenCVGenerator(QObject* parent, int videoIndex)
	: MediaGenerator(parent), mVideoIndex(videoIndex)
{}

OpenCVGenerator::~OpenCVGenerator()
{}

void OpenCVGenerator::setInputIndex(int index) {
	if (mVideoIndex == index) return; // Check if index is the same
	mVideoIndex = index;
	
	if (cap.isOpened()) cap.release();

	emit cameraIndexChanged(mVideoIndex);
}

void OpenCVGenerator::start() {
	cap.open(mVideoIndex, cv::CAP_DSHOW);

	// Start the frame grabbing loop
	nextFrame();
}

void OpenCVGenerator::stop()
{
	cap.release();
}

void OpenCVGenerator::nextFrame() {
	// Get mat
	cv::Mat mat;
	cap >> mat;

	// Check if the mat is empty
	if (mat.empty()) {
		//cap.release();
		return;
	}

	// Convert from BGR to BGRA
	cv::Mat matBGRA;
	cv::cvtColor(mat, matBGRA, cv::COLOR_BGR2BGRA);

	// Create a QImage that uses matBGRA's data
	QImage img(matBGRA.data, matBGRA.cols, matBGRA.rows, static_cast<int>(matBGRA.step), QImage::Format_ARGB32);

	// Make a deep copy so that the image owns its data
	QImage imgCopy = img.copy();

	// Create QVideoFrame
	QVideoFrame frame(imgCopy);
	emit frameReady(frame);
}