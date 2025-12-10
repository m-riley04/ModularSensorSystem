#pragma once
#include <QObject>
#include "idatasource.hpp"

class IAudioSource : public IDataSource {

public:
    virtual ~IAudioSource() = default;

};
Q_DECLARE_INTERFACE(IAudioSource, "com.modularsensorsystem.IAudioSource/1.0")