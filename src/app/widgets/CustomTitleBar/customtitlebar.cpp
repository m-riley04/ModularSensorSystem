#include "customtitlebar.h"
#include <QWindow>

CustomTitleBar::CustomTitleBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonClose, &QPushButton::clicked, this, &CustomTitleBar::onCloseClicked);
	connect(ui.buttonMaximize, &QPushButton::clicked, this, &CustomTitleBar::onMaximizeClicked);
	connect(ui.buttonMinimize, &QPushButton::clicked, this, &CustomTitleBar::onMinimizeClicked);

	// Init title
	setTitle(qApp->applicationDisplayName());
	this->setAttribute(Qt::WA_StyledBackground, true);

	// Init icons
	initIcons();
}

CustomTitleBar::~CustomTitleBar()
{}

void CustomTitleBar::mousePressEvent(QMouseEvent* ev) {
	if (ev->button() == Qt::LeftButton) {
		// Use native window move (Qt 5.15+): 
		// this asks the OS to start moving the window as if dragging the title bar.
		QWindow* pParentWindow = window()->windowHandle();
		if (pParentWindow) {
			pParentWindow->startSystemMove();  // native move (snap/tiling will work):contentReference[oaicite:1]{index=1}.
		}
	}
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

void CustomTitleBar::initIcons()
{
	// Attempt to load icons
	QString iconPath = ":/icons/media/icons/";
	QIcon appIcon(iconPath + "mss_template_icon.png");

	// TODO/CONSIDER: check if icon is valid? 
	
	// Set the icons
	setTitleIcon(appIcon);
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
