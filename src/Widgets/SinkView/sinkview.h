#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_sinkview.h"
#include "Processing/yolo.h"

class SinkView : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::SinkViewClass ui;
	QVideoFrame mFrame;
	std::unique_ptr<QVideoSink> pSink = nullptr;
	std::unique_ptr<Yolo> pYolo = nullptr;

	std::vector<std::string> mClasses;
	std::vector<Yolo::Detection> mDetections;

	void captureFrame();
	void activateYOLO();

public:
	SinkView(QWidget *parent = nullptr);
	~SinkView();

	QVideoSink* sink() const { return pSink.get(); }
	void setVideoFrame(const QVideoFrame& frame);
	
public slots:
	void receiveDetections(std::vector<Yolo::Detection> detections);

signals:
	void sendNewFrameYolo(QImage imageFrame);
};
