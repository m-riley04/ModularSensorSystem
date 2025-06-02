#pragma once

#include <QWidget>
#include "ui_sourceswidget.h"
#include <qmessagebox.h>
#include "dialogs/AddSourceDialog/addsourcedialog.h"
#include <Controllers/SourceController/sourcecontroller.h>
#include <sources/IConfigurableSource/iconfigurablesource.h>
#include <qobject.h>
#include <QMimeData>

class SourcesWidget : public QWidget
{
	Q_OBJECT

public:
	SourcesWidget(QWidget* parent = nullptr);
	SourcesWidget(SourceController* sourceController, QWidget *parent = nullptr);
	~SourcesWidget();

private:
	Ui::SourcesWidgetClass ui;
	SourceController* pSourceController;

	void initSignals();
	void initDeviceControllerSignals();

public slots:
	void openAddSourceDialog();
	void openRemoveSourceDialog();
	void openProperties();
	void setSourceController(SourceController* sourceController);

signals:
	void sourceControllerChanged(SourceController* sourceController);
};
