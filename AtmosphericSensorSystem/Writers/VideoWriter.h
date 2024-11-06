#pragma once

#include "SensorWriter.h"
#include <opencv2/opencv.hpp>
#include <QVariant>

class VideoWriter : public SensorWriter
{
	Q_OBJECT

private:
	cv::VideoWriter writer;
	std::string filename;
	int fourcc;
	double fps;
	cv::Size frameSize;


public:
	VideoWriter(const std::string& filename, int fourcc, double fps, cv::Size frameSize);
	~VideoWriter();

	bool isOpened() const;

public slots:
	void record(const QVariant& data) override;
};
