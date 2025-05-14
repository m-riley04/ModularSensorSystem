#include "previewcontainerwidget.h"

PreviewContainerWidget::PreviewContainerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PreviewContainerWidget::~PreviewContainerWidget()
{}

void PreviewContainerWidget::addVideoWidget(CameraDevice* camera)
{
	if (camera == nullptr) return;
	// Create a new video widget
	CustomSinkWidget* videoWidget = new CustomSinkWidget(this);

	// Add video widget to list
	mVideoWidgets.append(videoWidget);

	// Set camera and get props
	QString name = camera->camera()->cameraDevice().description();
	camera->setVideoOutput(videoWidget);

	// Add tab to widget
	ui.tabDevicePreviews->addTab(videoWidget, name);

	emit videoWidgetAdded(videoWidget);
}