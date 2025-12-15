#pragma once
#include <QJsonObject>

/**
 * @brief A capability interface for sources that can load and save configuration settings.
 */
class IConfigurableSource {
public:
    virtual ~IConfigurableSource() = default;
    
    /**
     * Loads the settings from the given JSON object.
     */
    virtual void loadSettings(const QJsonObject& obj) { Q_UNUSED(obj) }

    /**
     * Saves the settings to the given JSON object
     */
    virtual QJsonObject saveSettings() = 0;
};

#define IConfigurableSource_iid "com.rileymeyerkorth.IConfigurableSource/1.0"
Q_DECLARE_INTERFACE(IConfigurableSource, IConfigurableSource_iid)