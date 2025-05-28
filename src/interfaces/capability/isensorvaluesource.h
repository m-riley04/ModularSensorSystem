#pragma once
#include <QObject>
#include "idatasource.h"

class ISensorValueSource : public IDataSource
{

public:
    virtual ~ISensorValueSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void sensorReceived(const double data);
};

Q_DECLARE_INTERFACE(ISensorValueSource, "com.modularsensorsystem.ISensorValueSource/1.0")