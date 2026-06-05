#include <QFormLayout>
#include <QVariant>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

#include "controllers/loggingcontroller.hpp"
#include "../../widgets/widgets/QDirectoryPickerWidget/qdirectorypickerwidget.h"
#include "settingspagefactory.hpp"

QWidget* SettingsPageFactory::build(ISettingsProvider* provider, QWidget* parent) {
    auto* page = new QWidget(parent);
    auto* form = new QFormLayout(page);

    for (const auto& d : provider->settingsSchema()) {
        QWidget* editor = nullptr;
        const QVariant current = provider->settingValue(d.key);

        switch (d.type) {
        case SettingDescriptor::Bool: {
            auto* cb = new QCheckBox(page);
            cb->setChecked(current.toBool());
            QObject::connect(cb, &QCheckBox::toggled, page,
                [provider, key = d.key](bool v) { provider->setSettingValue(key, v); });
            editor = cb;
            break;
        }
        case SettingDescriptor::Int: {
            auto* sb = new QSpinBox(page);
            sb->setRange(d.minValue.toInt(), d.maxValue.toInt());
            sb->setValue(current.toInt());
            QObject::connect(sb, qOverload<int>(&QSpinBox::valueChanged), page,
                [provider, key = d.key](int v) { provider->setSettingValue(key, v); });
            editor = sb;
            break;
        }
        case SettingDescriptor::Enum: {
            auto* combo = new QComboBox(page);
            combo->addItems(d.enumOptions);
            combo->setCurrentText(current.toString());
            QObject::connect(combo, &QComboBox::currentTextChanged, page,
                [provider, key = d.key](const QString& v) { provider->setSettingValue(key, v); });
            editor = combo;
            break;
        }
        case SettingDescriptor::String: {
            // TODO
            break;
        }
        case SettingDescriptor::Double: {
            // TODO
            break;
        }
        case SettingDescriptor::FilePath: {
            // TODO
            break;
        }
        case SettingDescriptor::DirPath: {
            auto* picker = new QDirectoryPickerWidget(page);
            // convert string to dir if possible
            QDir dir = QDir(current.toString());
            if (!dir.exists()) {
                LoggingController::warning(QString("Could not convert value %1 to QDir").arg(d.defaultValue.toString()));
                break;
            }
            picker->setSelectedDirectory(dir);
            // TODO: make sure signal params are correct
            QObject::connect(picker, &QDirectoryPickerWidget::directoryChanged, page,
                [provider, key = d.key](const QDir& v) { provider->setSettingValue(key, v.absolutePath()); });
            editor = picker;
            break;
        }
        }

        if (editor) {
            editor->setToolTip(d.description);
            form->addRow(d.displayName, editor);
        }
    }

    // add button for default os settings
    QPushButton* button = new QPushButton(page);
    button->setText("Open OS Settings");
    form->addRow(button);

    QObject::connect(button, &QPushButton::clicked, [provider, parent]() { 
        if (!provider->openOsSettings()) {
            QMessageBox::warning(parent, "Error", "Unable to open OS settings. Please check the logs for more information.");
        }
        });

    return page;
}