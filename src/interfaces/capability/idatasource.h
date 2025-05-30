#pragma once
#include <QObject>

class IDataSource
{

public:
    virtual ~IDataSource() = default;

};

Q_DECLARE_INTERFACE(IDataSource, "com.modularsensorsystem.IDataSource/1.0")