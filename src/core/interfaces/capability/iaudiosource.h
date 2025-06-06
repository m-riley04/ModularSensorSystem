#pragma once
#include <QObject>
#include "idatasource.h"

class IAudioSource : public IDataSource
{

public:
    virtual ~IAudioSource() = default;

    virtual QObject* asQObject() = 0;
signals:
    void audioChunkReceived(const QByteArray&);
};

Q_DECLARE_INTERFACE(IAudioSource, "com.modularsensorsystem.IAudioSource/1.0")