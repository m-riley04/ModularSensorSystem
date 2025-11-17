#include "qdirectorypickerwidgetplugin.h"
#include "qdirectorypickerwidget.h"

#include <QtCore/QtPlugin>

QDirectoryPickerWidgetPlugin::QDirectoryPickerWidgetPlugin(QObject* parent)
    : QObject(parent)
{
    initialized = false;
}

void QDirectoryPickerWidgetPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QDirectoryPickerWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget* QDirectoryPickerWidgetPlugin::createWidget(QWidget* parent)
{
    return new QDirectoryPickerWidget(parent);
}

QString QDirectoryPickerWidgetPlugin::name() const
{
    return "QActionButton";
}

QString QDirectoryPickerWidgetPlugin::group() const
{
    return "ModularSensorSystem";
}

QIcon QDirectoryPickerWidgetPlugin::icon() const
{
    return QIcon();
}

QString QDirectoryPickerWidgetPlugin::toolTip() const
{
    return QString();
}

QString QDirectoryPickerWidgetPlugin::whatsThis() const
{
    return QString();
}

bool QDirectoryPickerWidgetPlugin::isContainer() const
{
    return false;
}

QString QDirectoryPickerWidgetPlugin::domXml() const
{
    return
        "<ui language=\"c++\">\n"
        "  <widget class=\"QActionButton\" name=\"actionButton\">\n"
        "    <property name=\"geometry\">\n"
        "      <rect>\n"
        "        <x>0</x>\n"
        "        <y>0</y>\n"
        "        <width>100</width>\n"
        "        <height>100</height>\n"
        "      </rect>\n"
        "    </property>\n"
        "    <property name=\"toolTip\">\n"
        "      <string></string>\n"
        "    </property>\n"
        "    <property name=\"whatsThis\">\n"
        "      <string></string>\n"
        "    </property>\n"
        "  </widget>\n"
        "</ui>\n";
}

QString QDirectoryPickerWidgetPlugin::includeFile() const
{
    return "qdirectorypickerwidget.h";
}