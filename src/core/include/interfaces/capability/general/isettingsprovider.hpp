#include <QString>
#include <QVariant>
#include <QString>

/**
 * @brief Struct for describing a single setting entry.
 * Has it's own type enum for the supported types of setting entries
 */
struct SettingDescriptor {
    enum Type { Bool, Int, Double, String, Enum, FilePath, DirPath, Color };

    Type type;
    QString key; // stable id, e.g. "frame_rate"
    QString displayName; // i.e. "Frame Rate"
    QString description; // tooltip / help text
    QString category; // optional grouping ("Capture", "Output", ...)
    QVariant defaultValue;
    QVariant minValue; // only used by numerical values currently
    QVariant maxValue;
    QStringList enumOptions; // for Enum
};

/**
 * @brief Capability interface for settings pages
 */
class ISettingsProvider {
public:
    virtual ~ISettingsProvider() = default;
    virtual QList<SettingDescriptor> settingsSchema() const = 0;
    virtual QVariant settingValue(const QString& key) const = 0;
    virtual bool setSettingValue(const QString& key, const QVariant& value) = 0;
    virtual void resetSettings() {}
    virtual bool openOsSettings() = 0;
};

Q_DECLARE_INTERFACE(ISettingsProvider, "com.modularsensorsystem.ISettingsProvider/1.0")