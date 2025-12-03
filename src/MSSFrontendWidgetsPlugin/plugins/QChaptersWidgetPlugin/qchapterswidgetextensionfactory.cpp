#include "qchapterswidgetextensionfactory.h"
#include "qchapterswidget.h"
#include "qchapterswidgetextension.h"

QChaptersWidgetExtensionFactory::QChaptersWidgetExtensionFactory(QExtensionManager* parent)
	: QExtensionFactory(parent)
{
}

QObject* QChaptersWidgetExtensionFactory::createExtension(QObject* object, const QString& iid, QObject* parent) const
{
    auto* widget = qobject_cast<QChaptersWidget*>(object);

    if (widget && (iid == Q_TYPEID(QDesignerContainerExtension)))
        return new QChaptersWidgetExtension(widget, parent);
    return nullptr;
}
