#include "qactionbuttonplugin.h"
#include "qactionbutton.h"

#include <QtCore/QtPlugin>

QActionButtonPlugin::QActionButtonPlugin(QObject* parent)
    : QObject(parent)
{
    initialized = false;
}

void QActionButtonPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QActionButtonPlugin::isInitialized() const
{
    return initialized;
}

QWidget* QActionButtonPlugin::createWidget(QWidget* parent)
{
    return new QActionButton(parent);
}

QString QActionButtonPlugin::name() const
{
    return "QActionButton";
}

QString QActionButtonPlugin::group() const
{
    return "ModularSensorSystem";
}

QIcon QActionButtonPlugin::icon() const
{
    return QIcon();
}

QString QActionButtonPlugin::toolTip() const
{
    return QString();
}

QString QActionButtonPlugin::whatsThis() const
{
    return QString();
}

bool QActionButtonPlugin::isContainer() const
{
    return false;
}

QString QActionButtonPlugin::domXml() const
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

QString QActionButtonPlugin::includeFile() const
{
    return "qactionbutton.h";
}