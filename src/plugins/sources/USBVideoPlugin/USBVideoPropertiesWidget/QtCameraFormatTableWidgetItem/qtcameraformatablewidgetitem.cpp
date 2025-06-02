#include "qtcameraformatablewidgetitem.h"
#include "USBVideoPropertiesWidget/usbvideopropertieswidget.h"

QtCameraFormatTableWidgetItem::QtCameraFormatTableWidgetItem(QString text, int type) : QTableWidgetItem(text, type)
{}

QtCameraFormatTableWidgetItem::~QtCameraFormatTableWidgetItem()
{}

bool QtCameraFormatTableWidgetItem::operator<(const QTableWidgetItem& other) const
{
	// Custom sorting for different item types
	switch (this->type()) {
		case FPS:
			// Sort by the fps value
			return (this->text().toFloat() < other.text().toFloat());

		case RESOLUTION:
			// Sort by the first number (width)
			return USBVideoPropertiesWidget::stringToSize(this->text()).width() < USBVideoPropertiesWidget::stringToSize(other.text()).width();

		case PIXEL_FORMAT:
			// Sort by the pixel format name
			return (this->text() < other.text());

		default:
			return QTableWidgetItem::operator<(other);
	}
}