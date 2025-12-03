#pragma once

#include <QObject>
#include <QExtensionFactory>

class QChaptersWidgetExtensionFactory : public QExtensionFactory
{
	Q_OBJECT

public:
    explicit QChaptersWidgetExtensionFactory(QExtensionManager* parent = nullptr);

protected:
    QObject* createExtension(QObject* object, const QString& iid, QObject* parent) const override;

};

