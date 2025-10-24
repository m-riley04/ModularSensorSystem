#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include <QList>
#include "controllers/maincontroller.h"
#include "features/sources/sourcepreview.h"
#include "widgets/SourcePreviewWidget/sourcepreviewwidget.h"

class PreviewContainerWidget : public QWidget
{
	Q_OBJECT

public:
	PreviewContainerWidget(QWidget *parent = nullptr);
	~PreviewContainerWidget();

	void setController(MainController* controller) { 
		if (pController == controller) return;
		pController = controller;
		initSignals();
	}

private:
	Ui::PreviewContainerWidgetClass ui;
	QList<SourcePreviewWidget*> mSourcePreviewWidgets;

	MainController* pController = nullptr;

	void initSignals();

public slots:
	void addSourceWidget(Source*);
	void removeSourceWidget(QUuid id);

signals:
	void sourceWidgetAdded(SourcePreviewWidget* widget);
	void sourceWidgetRemoved(SourcePreviewWidget* widget);

};
