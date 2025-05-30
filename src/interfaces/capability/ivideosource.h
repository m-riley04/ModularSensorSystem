#pragma once
#include <QObject>
#include "idatasource.h"
#include <qvideoframe.h>

class IVideoSource : public IDataSource
{

public:
    virtual ~IVideoSource() = default;
signals:
    void frameReady(const QVideoFrame&);
};

Q_DECLARE_INTERFACE(IVideoSource, "com.modularsensorsystem.IVideoSource/1.0")