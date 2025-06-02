#pragma once

#include <QObject>
#include "sources/Source/source.h"

class Device;

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
