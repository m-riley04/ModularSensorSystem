#include "customtitlebar.h"

CustomTitleBar::CustomTitleBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonClose, &QPushButton::clicked, this, &CustomTitleBar::onCloseClicked);
	connect(ui.buttonMaximize, &QPushButton::clicked, this, &CustomTitleBar::onMaximizeClicked);
	connect(ui.buttonMinimize, &QPushButton::clicked, this, &CustomTitleBar::onMinimizeClicked);
}

CustomTitleBar::~CustomTitleBar()
{}

void CustomTitleBar::mousePressEvent(QMouseEvent* event) {
	mMousePressPosition = event->globalPosition();
	mWindowPosition = pParentWindow->frameGeometry().topLeft();

	event->accept();
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent* event) {
	if (!mMousePressPosition.isNull()) {
		QPointF deltaFloat = event->globalPosition() - mMousePressPosition;
		QPoint delta = QPoint(deltaFloat.x(), deltaFloat.y());
		pParentWindow->move(mWindowPosition + delta);
	}
	event->accept();
}

void CustomTitleBar::mouseReleaseEvent(QMouseEvent* event) {
	mMousePressPosition = QPoint();
	event->accept();
}

void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (!pParentWindow) {
		qDebug() << "Parent widget is null, cannot toggle maximize.";
		return;
	}

	if (pParentWindow->isMaximized()) {
		pParentWindow->showNormal();
	}
	else {
		pParentWindow->showMaximized();
	}
}

void CustomTitleBar::onMinimizeClicked()
{
	if (!pParentWindow) {
		qDebug() << "Parent widget is null, cannot minimize.";
		return;
	}

	pParentWindow->showMinimized();
}

void CustomTitleBar::onMaximizeClicked()
{
	if (!pParentWindow) {
		qDebug() << "Parent widget is null, cannot maximize.";
		return;
	}

	if (pParentWindow->isMaximized()) {
		pParentWindow->showNormal();
	} else {
		pParentWindow->showMaximized();
	}
}

void CustomTitleBar::onCloseClicked()
{
	qApp->quit();
}
