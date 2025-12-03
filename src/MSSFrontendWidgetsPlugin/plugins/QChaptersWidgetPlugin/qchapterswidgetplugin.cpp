#include "qchapterswidgetplugin.h"
#include "qchapterswidget.h"
#include "qchapterswidgetextensionfactory.h"

#include <QtCore/QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QDesignerPropertySheetExtension>
#include <QDesignerFormWindowInterface>
#include <QDesignerContainerExtension>
#include <QExtensionManager>

QChaptersWidgetPlugin::QChaptersWidgetPlugin(QObject* parent)
    : QObject(parent)
{
    initialized = false;
}

void QChaptersWidgetPlugin::initialize(QDesignerFormEditorInterface* formEditor)
{
    if (initialized) return;

    auto* manager = formEditor->extensionManager();
    auto* factory = new QChaptersWidgetExtensionFactory(manager);

    Q_ASSERT(manager != nullptr);
    manager->registerExtensions(factory, Q_TYPEID(QDesignerContainerExtension));

    initialized = true;
}

void QChaptersWidgetPlugin::currentIndexChanged(int index)
{
    auto* widget = qobject_cast<QChaptersWidget*>(sender()); if (!widget) return;
    auto* form = QDesignerFormWindowInterface::findFormWindow(widget); if (!form) return;
    if (auto* page = widget->widget(index)) {
        form->clearSelection();
        form->selectWidget(page); // select the content page
    }
    form->emitSelectionChanged();
}

void QChaptersWidgetPlugin::pageTitleChanged(int index, const QString& title)
{
    Q_UNUSED(title);
    auto* widget = qobject_cast<QChaptersWidget*>(sender()); if (!widget) return;
    auto* page = widget->widget(widget->currentIndex()); if (!page) return;
    auto* form = QDesignerFormWindowInterface::findFormWindow(widget); if (!form) return;
	auto* editor = form->core(); if (!editor) return;
	auto* manager = editor->extensionManager(); if (!manager) return;
    auto* sheet = qt_extension<QDesignerPropertySheetExtension*>(manager, page); if (!sheet) return;
    const int propertyIndex = sheet->indexOf(QLatin1String("windowTitle"));
    sheet->setChanged(propertyIndex, true);
}

void QChaptersWidgetPlugin::pageTitleVisibleChanged(int index, bool visible)
{
    Q_UNUSED(visible);
    auto* widget = qobject_cast<QChaptersWidget*>(sender());
    if (!widget) return;
    auto* form = QDesignerFormWindowInterface::findFormWindow(widget);
    if (!form) return;
    auto* editor = form->core();
    auto* manager = editor->extensionManager();
    auto* sheet = qt_extension<QDesignerPropertySheetExtension*>(manager, widget);
    const int propertyIndex = sheet->indexOf(QLatin1String("pageTitleVisible"));
	sheet->setChanged(propertyIndex, true);
}

bool QChaptersWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget* QChaptersWidgetPlugin::createWidget(QWidget* parent)
{
	auto widget = new QChaptersWidget(parent);
    connect(widget, &QChaptersWidget::currentIndexChanged,
        this, &QChaptersWidgetPlugin::currentIndexChanged);
    connect(widget, &QChaptersWidget::pageTitleChanged,
        this, &QChaptersWidgetPlugin::pageTitleChanged);
	connect(widget, &QChaptersWidget::pageTitleVisibleChanged,
		this, &QChaptersWidgetPlugin::pageTitleVisibleChanged);
    return widget;
}

QString QChaptersWidgetPlugin::name() const
{
    return "QChaptersWidget";
}

QString QChaptersWidgetPlugin::group() const
{
    return "ModularSensorSystem";
}

QIcon QChaptersWidgetPlugin::icon() const
{
    return QIcon();
}

QString QChaptersWidgetPlugin::toolTip() const
{
    return QString();
}

QString QChaptersWidgetPlugin::whatsThis() const
{
    return QString();
}

bool QChaptersWidgetPlugin::isContainer() const
{
    return true;
}

QString QChaptersWidgetPlugin::domXml() const
{
    return
        "<ui language=\"c++\">\n"
        "  <widget class=\"QChaptersWidget\" name=\"chaptersWidget\">\n"
        "    <widget class=\"QWidget\" name=\"page\"/>\n"
        "  </widget>\n"
        "  <customwidgets>\n"
        "    <customwidget>\n"
        "      <class>QChaptersWidget</class>\n"
        "      <extends>QFrame</extends>\n"
        "      <addpagemethod>addPage</addpagemethod>\n"
        "    </customwidget>\n"
        "  </customwidgets>\n"
        "</ui>\n";
}

QString QChaptersWidgetPlugin::includeFile() const
{
    return "qchapterswidget.h";
}

