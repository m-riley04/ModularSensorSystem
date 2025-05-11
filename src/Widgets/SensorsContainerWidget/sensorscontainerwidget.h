#pragma once

#include <QWidget>
#include "ui_sensorscontainerwidget.h"
#include "Widgets/SensorsContainerItemWidget/sensorscontaineritemwidget.h"

class SensorsContainerWidget : public QWidget
{
	Q_OBJECT

public:
	SensorsContainerWidget(QWidget *parent = nullptr);
	~SensorsContainerWidget();

	void addCameraWidget(CustomSinkWidget* widget)
	{
		auto itemWidget = new SensorsContainerItemWidget(this, widget);
		mSinkWidgets.append(itemWidget);
	}

	void removeCameraWidget(CustomSinkWidget* widget)
	{
		mSinkWidgets.removeIf([widget](SensorsContainerItemWidget* itemWidget) {
			return itemWidget->cameraSinkWidget() == widget;
			});

		// Clean up
		for (auto it = mSinkWidgets.begin(); it != mSinkWidgets.end(); ++it) {
			if ((*it)->cameraSinkWidget() == widget) {
				delete* it;
				mSinkWidgets.erase(it);
				break;
			}
		}
	}

private:
	Ui::SensorsContainerWidgetClass ui;

	QList<SensorsContainerItemWidget*> mSinkWidgets;
};
