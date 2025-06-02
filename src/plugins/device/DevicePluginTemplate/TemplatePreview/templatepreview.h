#pragma once

#include "devices/Device/DevicePreview/devicepreview.h"
#include <QObject>

class TemplatePreview : public SourcePreview
{
	Q_OBJECT

public:
	TemplatePreview(QObject *parent);
	~TemplatePreview();
};
