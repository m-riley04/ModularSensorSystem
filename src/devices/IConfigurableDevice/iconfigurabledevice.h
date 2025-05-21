#pragma once
#include <QWidget>

class IConfigurableDevice {
public:
    virtual ~IConfigurableDevice() = default;

    virtual QWidget* createConfigWidget(QWidget* parent = nullptr) = 0;
    
    virtual void loadSettings(const QJsonObject& obj) { Q_UNUSED(obj) }
    virtual void saveSettings(QJsonObject& obj) const { Q_UNUSED(obj) }
};

#define IConfigurableDevice_iid "com.rileymeyerkorth.IConfigurableDevice/1.0"
Q_DECLARE_INTERFACE(IConfigurableDevice, IConfigurableDevice_iid)