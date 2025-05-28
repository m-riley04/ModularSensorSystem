#pragma once
#include <QObject>
#include "idatasource.h"

class IAudioStreamSource : public IDataSource
{

public:
    virtual ~IAudioStreamSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void audioChunkReceived(const QByteArray&);
};

Q_DECLARE_INTERFACE(IAudioStreamSource, "com.modularsensorsystem.IAudioStreamSource/1.0")