#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_customsinkwidget.h"
#include "Processing/YOLO/yolo.h"
#include <QThread>
#include "devices/Device/device.h"

class CustomSinkWidget : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event) override;

public:
	/// <summary>
	/// Contains all the necessary values for calculating the frame size and position
	/// </summary>
	struct FrameSizeCalculations {
		double widgetRatio;
		double frameRatio;
		double actualFrameWidth;
		double actualFrameHeight;
		double widthScalingFactor;
		double heightScalingFactor;
		double xOffset;
		double yOffset;
	};

	/// <summary>
	/// Represents a simple debug item and value to be painted on the screen
	/// </summary>
	struct FrameDebugItem {
		QString label;
		QVariant value;
	};

private:
	Ui::CustomSinkWidgetClass ui;
	QVideoFrame mFrame;
	std::unique_ptr<QVideoSink> pSink = nullptr;

	bool mIsDebugInfoVisible = false;
	bool mIsDetectionActive = false;
	
	QThread* pYoloThread = nullptr;
	Yolo* pYolo = nullptr;
	std::vector<std::string> mClasses;
	std::vector<Yolo::Detection> mDetections;
	int mCaptureIntervalMs = 30;

	Device* pDevice = nullptr;

	void initializeYolo();
	void activateYOLO();

	FrameSizeCalculations calculateFrameSizings();
	void captureFrame();
	
	void paintDebugInfo(QPainter& painter, FrameSizeCalculations& calculations);
	void paintDetections(QPainter& painter, FrameSizeCalculations& sizing, QRectF& outputRect);

public:
	CustomSinkWidget(Device* device, QWidget *parent = nullptr);
	~CustomSinkWidget();

	Device* device() const { return pDevice; }

	Yolo* yolo() const { return pYolo; }
	bool detectionState() const { return mIsDetectionActive; }
	bool isDebugInfoVisible() const { return mIsDebugInfoVisible; }
	QVideoSink* videoSink() const { return pSink.get(); }
	void setVideoFrame(const QVideoFrame& frame);
	
public slots:
	void receiveDetections(std::vector<Yolo::Detection> detections);
	void setDetectionState(bool state);
	void setDebugInfoVisible(bool visible);

signals:
	void sendNewFrameYolo(QImage imageFrame);
	void detectionStateChanged(bool state);
	void debugInfoVisibleChanged(bool visible);
};
