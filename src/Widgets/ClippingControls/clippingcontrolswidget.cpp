#include "clippingcontrolswidget.h"
#include <qfiledialog.h>

ClippingControlsWidget::ClippingControlsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.frameButtons->setEnabled(false);
	ui.frameParameters->setEnabled(false);

	// Connect signals and slots for UI elements
	connect(ui.buttonToggleClipping, &QPushButton::clicked, this, [this]() {
		setClippingEnabled(!mClippingEnabled);
		if (pVideoBuffer) {
			pVideoBuffer->setBuffering(mClippingEnabled);
		}
		});
	connect(ui.buttonClip, &QPushButton::clicked, this, [this]() {
		QString fileName = QFileDialog::getSaveFileName(this, "Save video", QDir::currentPath(), "Video files (*.mp4)");
		if (!fileName.isEmpty() && pClipController) {
			pClipController->saveClip(fileName);
			emit clipSaved(fileName);
		}
		});
	connect(ui.buttonClearBuffer, &QPushButton::clicked, this, [this]() {
		if (pVideoBuffer) {
			pVideoBuffer->clear();
			emit bufferCleared();

			updateUiElements();
		}
		});

	connectSignals();
}

ClippingControlsWidget::~ClippingControlsWidget()
{}

void ClippingControlsWidget::setVideoBuffer(VideoClipBuffer* buffer) {
	if (pVideoBuffer == buffer) return; // No change
	pVideoBuffer = buffer;
	if (pClipController) {
		delete pClipController; // Clean up old controller
	}

	pClipController = new ClipController(buffer, 640, 480, 30.0, 4000000, this);

	// Update signals
	connectSignals();

	// Update UI elements
	updateUiElements();
}

void ClippingControlsWidget::setClippingEnabled(bool enabled) {
	if (mClippingEnabled == enabled) return; // No change
	mClippingEnabled = enabled;

	// Emit signal to indicate clipping state change
	emit clippingStarted();

	// Update UI elements
	updateUiElements();
}

void ClippingControlsWidget::updateUiElements()
{
	ui.frameButtons->setEnabled(pVideoBuffer && pClipController);
	ui.frameParameters->setEnabled(pVideoBuffer && pClipController);
	ui.buttonToggleClipping->setText(mClippingEnabled ? "Stop Clipping" : "Start Clipping");
	ui.labelMaxFramesQueued->setText(QString("%1").arg(pVideoBuffer ? pVideoBuffer->size() : 0));
	ui.labelFramesQueued->setText(QString(pVideoBuffer ? "%1" : "0").arg(pVideoBuffer->size())); // TODO: This is supposed to be only current frames queued. Add method.
}

void ClippingControlsWidget::connectSignals()
{
	connect(pVideoBuffer, &VideoClipBuffer::frameAdded, this, [this]() {
		if (pVideoBuffer) {
			ui.labelFramesQueued->setText(QString("%1").arg(pVideoBuffer->size())); // TODO: This is supposed to be only current frames queued. Add method.
		}
		});
}
