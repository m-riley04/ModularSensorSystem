#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_sinkview.h"
#include "Processing/yolo.h"
#include <QThread>

class SinkView : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::SinkViewClass ui;
	QVideoFrame mFrame;
	std::unique_ptr<QVideoSink> pSink = nullptr;
	
	QThread* pYoloThread = nullptr;
	bool isDetectionActive = false;
	int captureIntervalMs = 30;
	Yolo* pYolo = nullptr;
	std::vector<std::string> mClasses;
	std::vector<Yolo::Detection> mDetections;

	void initializeYolo();

	void captureFrame();
	void activateYOLO();

public:
	SinkView(QWidget *parent = nullptr);
	~SinkView();

	QVideoSink* sink() const { return pSink.get(); }
	void setVideoFrame(const QVideoFrame& frame);
	
public slots:
	void receiveDetections(std::vector<Yolo::Detection> detections);
	void setDetectionState(bool state);

signals:
	void sendNewFrameYolo(QImage imageFrame);
	void detectionStateChanged(bool state);
};
