#pragma once
#include <QObject>
#include "idatasource.h"

class IGpsSource : public IDataSource
{

public:
    virtual ~IGpsSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void nmeaReceived(const QByteArray&);
};

Q_DECLARE_INTERFACE(IGpsSource, "com.modularsensorsystem.IGpsSource/1.0")