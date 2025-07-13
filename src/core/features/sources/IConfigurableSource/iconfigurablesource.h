#pragma once

class IConfigurableSource {
public:
    virtual ~IConfigurableSource() = default;
    
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

#define IConfigurableSource_iid "com.rileymeyerkorth.IConfigurableSource/1.0"
Q_DECLARE_INTERFACE(IConfigurableSource, IConfigurableSource_iid)