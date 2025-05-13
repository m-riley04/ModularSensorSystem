#pragma once

#include <QWidget>
#include "ui_clippingcontrolswidget.h"
#include "Controllers/ClipController/clipcontroller.h"
#include "Clipping/Buffers/VideoClipBuffer/videoclipbuffer.h"

class ClippingControlsWidget : public QWidget
{
	Q_OBJECT

public:
	ClippingControlsWidget(QWidget *parent = nullptr);
	~ClippingControlsWidget();

	ClipController* clipController() const { return pClipController; }
	VideoClipBuffer* videoBuffer() const { return pVideoBuffer; }

	void setVideoBuffer(VideoClipBuffer* buffer);
	void setClippingEnabled(bool enabled);

private:
	Ui::ClippingControlsWidgetClass ui;

	ClipController* pClipController = nullptr;
	VideoClipBuffer* pVideoBuffer = nullptr;
	bool mClippingEnabled = false;

	void updateUiElements();
	void connectSignals();

signals:
	void clipSaved(const QString& fileName);
	void clippingStarted();
	void clippingStopped();
	void bufferCleared();
	void clipControllerChanged(const ClipController* controller);
};
