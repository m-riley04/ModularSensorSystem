#pragma once
#include <QJsonObject>

// Forward declare QWidget to avoid QtWidgets dependency in core
// TODO: actually remove QWidget entirely (later)
class QWidget;

class IConfigurableSource {
public:
    virtual ~IConfigurableSource() = default;

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

#define IConfigurableSource_iid "com.rileymeyerkorth.IConfigurableSource/1.0"
Q_DECLARE_INTERFACE(IConfigurableSource, IConfigurableSource_iid)