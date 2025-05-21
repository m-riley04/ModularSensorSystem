#pragma once

#include <QObject>
#include "devices/Device/device.h"
#include "TemplateClipBuffer/templateclipbuffer.h"
#include "TemplatePreview/templatepreview.h"

class TemplateDevice : public Device
{
	Q_OBJECT

public:
	TemplateDevice(QObject *parent);
	~TemplateDevice();

public slots:
    void open() override;
    void start() override;
    void stop() override;
	void close() override;
    void restart() override;

	void beginRecording(RecordingSession*) override;
	void endRecording() override;
};
