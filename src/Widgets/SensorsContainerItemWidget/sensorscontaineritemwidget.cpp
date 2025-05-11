#include "sensorscontaineritemwidget.h"

SensorsContainerItemWidget::SensorsContainerItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

SensorsContainerItemWidget::SensorsContainerItemWidget(QWidget* parent, CustomSinkWidget* cameraSinkWidget)
	: QWidget(parent), pCameraSinkWidget(cameraSinkWidget)
{
	ui.setupUi(this);
}

SensorsContainerItemWidget::~SensorsContainerItemWidget()
{}

void SensorsContainerItemWidget::setWidget(CustomSinkWidget* widget)
{
	if (pCameraSinkWidget == widget) return;
	pCameraSinkWidget = widget;

	emit widgetChanged(widget);
}

void SensorsContainerItemWidget::mousePressEvent(QMouseEvent * event)
{
	// Check if the left mouse button was pressed
	if (event->button() == Qt::LeftButton) {
		if (event->button() == Qt::LeftButton)
			dragStartPosition = event->pos();
	}
}

void SensorsContainerItemWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->pos() - dragStartPosition).manhattanLength()
		< QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;

	//mimeData->setData(mimeType, data);
	mimeData->setText("Sensor Data");
	drag->setMimeData(mimeData);

	Qt::DropAction dropAction = drag->exec();
}
