#include "groupselectwidgetplugin.h"
#include "groupselectwidget.h"

#include <QtCore/QtPlugin>

GroupSelectWidgetPlugin::GroupSelectWidgetPlugin(QObject* parent)
	: QObject(parent)
{
	initialized = false;
}

void GroupSelectWidgetPlugin::initialize(QDesignerFormEditorInterface*)
{
	if (initialized) return;
	initialized = true;
}

bool GroupSelectWidgetPlugin::isInitialized() const
{
	return initialized;
}

QWidget* GroupSelectWidgetPlugin::createWidget(QWidget* parent)
{
	return new GroupSelectWidget(parent);
}

QString GroupSelectWidgetPlugin::name() const
{
	return "GroupSelectWidget";
}

QString GroupSelectWidgetPlugin::group() const
{
	return "ModularSensorSystem";
}

QIcon GroupSelectWidgetPlugin::icon() const
{
	return QIcon();
}

QString GroupSelectWidgetPlugin::toolTip() const
{
	return "Multi-select dropdown";
}

QString GroupSelectWidgetPlugin::whatsThis() const
{
	return QString();
}

bool GroupSelectWidgetPlugin::isContainer() const
{
	return false;
}

QString GroupSelectWidgetPlugin::domXml() const
{
	return
		"<ui language=\"c++\">\n"
		"  <widget class=\"GroupSelectWidget\" name=\"groupSelectWidget\">\n"
		"    <property name=\"geometry\">\n"
		"      <rect>\n"
		"        <x>0</x>\n"
		"        <y>0</y>\n"
		"        <width>150</width>\n"
		"        <height>24</height>\n"
		"      </rect>\n"
		"    </property>\n"
		"  </widget>\n"
		"</ui>\n";
}

QString GroupSelectWidgetPlugin::includeFile() const
{
	return "groupselectwidget.h";
}
