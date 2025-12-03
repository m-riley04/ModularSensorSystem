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
    Q_UNUSED(index);
    auto* widget = qobject_cast<QChaptersWidget*>(sender());
    if (widget) {
        auto* form = QDesignerFormWindowInterface::findFormWindow(widget);
        if (form)
            form->emitSelectionChanged();
    }
}

void QChaptersWidgetPlugin::pageTitleChanged(int index, const QString& title)
{
    Q_UNUSED(title);
    auto* widget = qobject_cast<QChaptersWidget*>(sender());
    if (widget) {
        auto* page = widget->widget(widget->currentIndex());
        auto* form = QDesignerFormWindowInterface::findFormWindow(widget);
        auto* editor = form->core();
        auto* manager = editor->extensionManager();
        auto* sheet = qt_extension<QDesignerPropertySheetExtension*>(manager, page);
        const int propertyIndex = sheet->indexOf(QLatin1String("windowTitle"));
        sheet->setChanged(propertyIndex, true);
    }
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
    connect(widget, &QChaptersWidget::chapterTitleChanged,
        this, &QChaptersWidgetPlugin::pageTitleChanged);
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
        "    <property name=\"geometry\">\n"
        "      <rect>\n"
        "        <x>0</x>\n"
        "        <y>0</y>\n"
        "        <width>600</width>\n"
        "        <height>300</height>\n"
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

QString QChaptersWidgetPlugin::includeFile() const
{
    return "qchapterswidget.h";
}

