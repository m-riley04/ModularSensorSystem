#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include <QList>
#include "controllers/maincontroller.h"
#include "widgets/SourcePreviewWidget/sourcepreviewwidget.h"

class PreviewContainerWidget : public QWidget
{
	Q_OBJECT

public:
	PreviewContainerWidget(QWidget *parent = nullptr);
	~PreviewContainerWidget();

	void setController(MainController* controller);

public slots:
	void addSourceWidget(Source*);
	void removeSourceWidget(QUuid id);

	void updateButtonControls();

private:
	Ui::PreviewContainerWidgetClass ui;
	QList<SourcePreviewWidget*> m_sourcePreviewWidgets;
	QList<QDynamicGridWidget*> m_gridWidgets;

	MainController* m_controller = nullptr;

	void initSignals();

signals:
	void sourceWidgetAdded(SourcePreviewWidget* widget);
	void sourceWidgetRemoved(SourcePreviewWidget* widget);

};
