#pragma once

#include <QObject>
#include <QtMultimedia>

class MediaGenerator  : public QObject
{
	Q_OBJECT

public:
	MediaGenerator(QObject *parent = nullptr);
	~MediaGenerator();

public slots:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void nextFrame() = 0;

signals:
	void frameReady(const QVideoFrame& frame);
};
