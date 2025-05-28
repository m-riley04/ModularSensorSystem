#pragma once
#include <QObject>
#include "idatasource.h"

class IGpsSentenceSource : public IDataSource
{

public:
    virtual ~IGpsSentenceSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void nmeaReceived(const QByteArray&);
};

Q_DECLARE_INTERFACE(IGpsSentenceSource, "com.modularsensorsystem.IGpsSentenceSource/1.0")