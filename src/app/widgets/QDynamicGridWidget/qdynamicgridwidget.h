#pragma once

#include <QWidget>
#include "ui_qdynamicgridwidget.h"

class QDynamicGridWidget : public QWidget
{
	Q_OBJECT

public:
	QDynamicGridWidget(QWidget *parent = nullptr);
	~QDynamicGridWidget();

public slots:
    /**
	 * Adds a widget to the grid layout.
	 * @param widget
     */
    void addWidgetToGrid(QWidget* widget);

	/**
	 * Removes a widget from the grid layout.
	 * @param widget 
	 */
	void removeWidgetFromGrid(QWidget* widget);

    /**
     * Updates the entire grid layout based on the current widgets.
     */
    void updateLayout();

private:
	Ui::QDynamicGridWidgetClass ui;

	QList<QWidget*> m_widgets;
};

