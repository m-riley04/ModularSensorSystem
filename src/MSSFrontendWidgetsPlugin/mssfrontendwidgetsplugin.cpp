#include "mssfrontendwidgetsplugin.h"
#include "qactionbuttonplugin.h"
#include "qdirectorypickerwidgetplugin.h"

#include <QtCore/QtPlugin>

MSSFrontendWidgetsPlugin::MSSFrontendWidgetsPlugin(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new QActionButtonPlugin(this));
    m_widgets.append(new QDirectoryPickerWidgetPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> MSSFrontendWidgetsPlugin::customWidgets() const
{
    return m_widgets;
}