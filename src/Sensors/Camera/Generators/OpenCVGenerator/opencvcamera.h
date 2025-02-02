#pragma once

#include <QObject>
#include <QtMultimedia>
#include <opencv2/opencv.hpp>
#include <QImage>
#include "../mediagenerator.h"

class OpenCVGenerator : public MediaGenerator
{
	Q_OBJECT

private:
	int mVideoIndex = 0;

public:
	OpenCVGenerator(QObject* parent = nullptr, int videoIndex = 0);
	~OpenCVGenerator();
	cv::VideoCapture cap;

public slots:
	void start() override;
	void stop() override;
	void nextFrame() override;

	void setInputIndex(int index);

signals:
	void cameraIndexChanged(int index);
};
