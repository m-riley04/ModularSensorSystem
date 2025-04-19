#pragma once

#include <QWidget>
#include "ui_clippingcontrolswidget.h"
#include <Controllers/ClipController/clipcontroller.h>

class ClippingControlsWidget : public QWidget
{
	Q_OBJECT

public:
	ClippingControlsWidget(QWidget *parent = nullptr);
	~ClippingControlsWidget();

	ClipController* clipController() const { return pClipController; }
	VideoBuffer* videoBuffer() const { return pVideoBuffer; }

	void setVideoBuffer(VideoBuffer* buffer);
	void setClippingEnabled(bool enabled);

private:
	Ui::ClippingControlsWidgetClass ui;

	ClipController* pClipController = nullptr;
	VideoBuffer* pVideoBuffer = nullptr;
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
