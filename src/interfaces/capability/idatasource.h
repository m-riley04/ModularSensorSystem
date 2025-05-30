#pragma once
#include <QObject>

class IDataSource: public QObject
{
    Q_OBJECT

public:
    virtual ~IDataSource() = default;
    virtual QObject* asQObject() = 0;        // helper for connect()

};

Q_DECLARE_INTERFACE(IDataSource, "com.modularsensorsystem.IDataSource/1.0")