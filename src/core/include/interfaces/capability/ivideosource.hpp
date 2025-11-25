#pragma once
#include <QObject>
#include "idatasource.hpp"
#include <qvideoframe.h>

class IVideoSource : public IDataSource {

public:
    virtual ~IVideoSource() = default;

};
Q_DECLARE_INTERFACE(IVideoSource, "com.modularsensorsystem.IVideoSource/1.0")