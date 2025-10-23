#pragma once

#include <QObject>
#include "source.h"

class Source;

class SourcePreview : public QObject
{
	Q_OBJECT

public:
	SourcePreview(QObject* parent);
	~SourcePreview() = default;

	Source* source() const { return m_source; }

private:
	Source* m_source = nullptr;

signals:
	void frameReady(const QImage& frame);
};
