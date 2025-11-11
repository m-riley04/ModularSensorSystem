#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include <QList>
#include "controllers/maincontroller.hpp"
#include "widgets/SourcePreviewWidget/sourcepreviewwidget.h"
#include "utils/boost_qt_conversions.hpp"

/**
 * A widget that contains and manages multiple QDynamicGridWidget instances to display SourcePreviewWidget instances.
 */
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
	QList<QDynamicGridWidget*> m_gridWidgets;

	MainController* m_controller = nullptr;

	void initSignals();

};
