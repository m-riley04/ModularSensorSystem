#pragma once
#include <QObject>
#include "idatasource.h"

class IAnalogSource : public IDataSource
{

public:
    virtual ~IAnalogSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void sensorReceived(const double data);
};

Q_DECLARE_INTERFACE(IAnalogSource, "com.modularsensorsystem.IAnalogSource/1.0")