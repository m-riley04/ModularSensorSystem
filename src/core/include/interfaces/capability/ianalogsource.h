#pragma once
#include <QObject>
#include "idatasource.h"
#include <QUuid>

struct AnalogDataSample
{
    QUuid sourceUuid;
    double value;
    quint64 tNs;
};

class IAnalogSource : public IDataSource
{

public:
    virtual ~IAnalogSource() = default;

signals:
    void analogDataReceived(AnalogDataSample data);
};
Q_DECLARE_INTERFACE(IAnalogSource, "com.modularsensorsystem.IAnalogSource/1.0")