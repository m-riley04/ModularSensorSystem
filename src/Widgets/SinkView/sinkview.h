#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_sinkview.h"

class SinkView : public QWidget
{
	Q_OBJECT

private:
	QVideoFrame mFrame;
	std::unique_ptr<QVideoSink> pSink = nullptr;

public:
	SinkView(QWidget *parent = nullptr);
	~SinkView();

	QVideoSink* sink() const { return pSink.get(); }
	void setVideoFrame(const QVideoFrame& frame);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::SinkViewClass ui;
};
