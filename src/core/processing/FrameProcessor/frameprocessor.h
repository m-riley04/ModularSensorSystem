#pragma once

#include <QObject>

class FrameProcessor  : public QObject
{
	Q_OBJECT

public:
	FrameProcessor(QObject *parent);
	~FrameProcessor();

	bool isEnabled() const { return mEnabled; }
	QString name() const { return mName; }
	qint64 lastProcessTime() const { return mLastProcessTime; }
	
private:
	bool mEnabled{ false };
	QString mName;
	qint64 mLastProcessTime{ 0 };

public slots:
	void setEnabled(bool enabled);
	void setName(const QString& name);
	void setLastProcessTime(qint64 time);

signals:
	void enabledChanged(bool enabled);
	void nameChanged(const QString& name);
	void lastProcessTimeChanged(qint64 time);
};
