#pragma once
#include <QObject>
#include "idatasource.h"
#include <QUuid>


struct GpsDataSample
{
    QUuid sourceUuid;
    QByteArray data;
    quint64 tNs;
};

class IGpsSource : public IDataSource {

public:
    virtual ~IGpsSource() = default;

signals:
    void nmeaReceived(GpsDataSample sample);
};

Q_DECLARE_INTERFACE(IGpsSource, "com.modularsensorsystem.IGpsSource/1.0")