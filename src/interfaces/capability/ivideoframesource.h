#pragma once
#include <QObject>
#include "idatasource.h"

class IVideoFrameSource : public IDataSource
{

public:
    virtual ~IVideoFrameSource() = default;
    
    virtual QObject* asQObject() = 0;
signals:
    void newFrame(const QImage&);
};

Q_DECLARE_INTERFACE(IVideoFrameSource, "com.modularsensorsystem.IVideoFrameSource/1.0")