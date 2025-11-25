#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

class MSSFrontendWidgetsPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" FILE "mssfrontendwidgetsplugin.json")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    MSSFrontendWidgetsPlugin(QObject *parent = nullptr);

    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};
