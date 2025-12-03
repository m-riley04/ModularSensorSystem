#include "mssfrontendwidgetsplugin.h"
#include "plugins/QActionButtonPlugin/qactionbuttonplugin.h"
#include "plugins/QDirectoryPickerWidgetPlugin/qdirectorypickerwidgetplugin.h"
#include "plugins/QChaptersWidgetPlugin/qchapterswidgetplugin.h"

#include <QtCore/QtPlugin>

MSSFrontendWidgetsPlugin::MSSFrontendWidgetsPlugin(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new QActionButtonPlugin(this));
    m_widgets.append(new QDirectoryPickerWidgetPlugin(this));
	m_widgets.append(new QChaptersWidgetPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> MSSFrontendWidgetsPlugin::customWidgets() const
{
    return m_widgets;
}