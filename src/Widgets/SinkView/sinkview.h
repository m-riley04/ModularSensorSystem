#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_sinkview.h"

class SinkView : public QWidget
{
	Q_OBJECT

private:
	QVideoSink sink;

public:
	SinkView(QWidget *parent = nullptr);
	~SinkView();

	QVideoSink videoSink() { return &sink; }

private:
	Ui::SinkViewClass ui;
};
