#pragma once

#include <QObject>
#include "features/sources/Source/source.h"

class Source;

class SourcePreview : public QObject
{
	Q_OBJECT

public:
	SourcePreview(QObject *parent);
	~SourcePreview();

	Source* source() const { return pSource; }

private:
	Source* pSource = nullptr;

signals:
	void frameReady(const QImage& frame);
};
