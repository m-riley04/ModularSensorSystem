#pragma once

#include <QWidget>
#include "ui_customtitlebar.h"
#include <qevent.h>
#include <QPoint>
#include <qmainwindow.h>
#include <QPointer>

class CustomTitleBar : public QWidget
{
	Q_OBJECT

public:
	CustomTitleBar(QWidget *parent = nullptr);
	~CustomTitleBar();

	void setParentWindow(QMainWindow* parent) {
		pParentWindow = parent;
	}

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	Ui::CustomTitleBarClass ui;
    QPointer<QMainWindow> pParentWindow; // Pointer to the main window of the app (uses it's positioning and movement)
    QPointF mMousePressPosition;
    QPoint mWindowPosition;

private slots:
	void onMinimizeClicked();
	void onMaximizeClicked();
	void onCloseClicked();

public slots:
	void setTitle(const QString& title) {
		ui.labelTitle->setText(title);
	}
	void setTitleIcon(const QIcon& icon) {
		ui.labelIcon->setPixmap(icon.pixmap(16, 16));
	}
};
