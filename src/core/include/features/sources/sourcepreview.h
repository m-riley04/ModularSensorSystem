#pragma once

#include <QObject>
#include <QUuid>

class SourcePreview : public QObject
{
	Q_OBJECT

public:
	SourcePreview(QUuid sourceId, QObject* parent) : QObject(parent), m_sourceId(sourceId) {}
	~SourcePreview() = default;

	QUuid sourceId() const { return m_sourceId; }

private:
	QUuid m_sourceId;

signals:
	void frameReady(const QImage& frame);
};
