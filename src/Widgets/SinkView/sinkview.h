#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_sinkview.h"
#include "Processing/yolo.h"

class SinkView : public QWidget
{
	Q_OBJECT

private:
	QVideoFrame mFrame;
	std::unique_ptr<QVideoSink> pSink = nullptr;
	std::unique_ptr<Yolo> pYolo = nullptr;

	std::vector<std::string> mClasses;
	const float mConfidenceThreshold{ 0.5 }; // Confidence threshold
	const float mNonMaximumSuppressionThreshold{ 0.4 };  // Non-maximum suppression threshold
	std::vector<Mat> m_outs;
	Mat mCurrentMat;

	void captureFrame();
	void postProcess(QVideoFrame frame, const vector<Mat>& outs);
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
	void activateYOLO();
	void receiveNewParameters(vector<Mat> outs_);

public:
	SinkView(QWidget *parent = nullptr);
	~SinkView();

	QVideoSink* sink() const { return pSink.get(); }
	void setVideoFrame(const QVideoFrame& frame);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::SinkViewClass ui;

signals:
	void sendFrame(QImage imageFrame);
	void sendNewFrameYolo(Mat frame);
};
