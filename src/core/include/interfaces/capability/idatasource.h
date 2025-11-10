#pragma once
#include <QObject>

class IDataSource
{

public:
    virtual ~IDataSource() = default;

    virtual QObject* asQObject() = 0;
};
Q_DECLARE_INTERFACE(IDataSource, "com.modularsensorsystem.IDataSource/1.0")