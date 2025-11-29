#pragma once

#include <QVariant>
#include <QAbstractItemModel>
#include "controllers/mountcontroller.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "features/mounts/mount.hpp"
#include "controllers/maincontroller.hpp"
#include "controllers/sessioncontroller.hpp"
#include <utils/plugins_utils.hpp>

class PluginListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PluginListModel(PluginController&, QObject* parent = nullptr);
    ~PluginListModel();

    // --- mandatory overrides ---
    QModelIndex index(int row, int col, const QModelIndex& p) const override;
    QModelIndex parent(const QModelIndex& c)  const override;
    int rowCount(const QModelIndex& p) const override;
	int columnCount(const QModelIndex&)   const override { return 5; } // name, author, version, type, isCore
    QVariant data(const QModelIndex&, int role) const override;

public slots:
    void rebuild();

private:
    PluginController& m_pluginController;
    QList<IPlugin*> m_plugins;

    void buildFlat();

};

Q_DECLARE_METATYPE(PluginListModel*)