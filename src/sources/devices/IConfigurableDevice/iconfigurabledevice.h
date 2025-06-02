#pragma once
#include <QWidget>

class IConfigurableDevice {
public:
    virtual ~IConfigurableDevice() = default;

    virtual QWidget* createConfigWidget(QWidget* parent = nullptr) = 0;
    
    /// <summary>
    /// Loads the settings from the given JSON object.
    /// </summary>
    /// <param name="obj"></param>
    virtual void loadSettings(const QJsonObject& obj) { Q_UNUSED(obj) }

    /// <summary>
    /// Saves the settings to the given JSON object
    /// </summary>
    /// <param name="obj"></param>
    virtual QJsonObject saveSettings() = 0;
};

#define IConfigurableDevice_iid "com.rileymeyerkorth.IConfigurableDevice/1.0"
Q_DECLARE_INTERFACE(IConfigurableDevice, IConfigurableDevice_iid)