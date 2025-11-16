#include "qactionbuttonplugin.h"

QActionButtonPlugin::QActionButtonPlugin(QObject* parent)
    : QObject(parent)
{}

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
    return QStringLiteral("QActionButton");
}

QString QActionButtonPlugin::group() const
{
    return QStringLiteral("Display Widgets [Examples]");
}

QIcon QActionButtonPlugin::icon() const
{
    return {};
}

QString QActionButtonPlugin::toolTip() const
{
    return {};
}

QString QActionButtonPlugin::whatsThis() const
{
    return {};
}

bool QActionButtonPlugin::isContainer() const
{
    return false;
}

QString QActionButtonPlugin::domXml() const
{
    return QString(
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
        "</ui>\n"
    );
}

QString QActionButtonPlugin::includeFile() const
{
    return QStringLiteral("qactionbutton.h");
}