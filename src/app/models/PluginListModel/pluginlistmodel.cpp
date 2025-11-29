#include "pluginlistmodel.h"

PluginListModel::PluginListModel(PluginController& c,
    QObject* parent)
    : QAbstractItemModel(parent), m_pluginController(c)
{
    // TODO: connect signals from plugin controller
    //connect(&m_pluginController, &PluginController::pluginsUpdated, this, &PluginListModel::rebuild);
}

PluginListModel::~PluginListModel()
{
}

QModelIndex PluginListModel::index(int row, int col, const QModelIndex& p) const
{
    if (col < 0 || col >= columnCount(p)) return {};
    if (row < 0 || row >= m_plugins.size()) return {};
	return createIndex(row, col, m_plugins[row]);
}

QModelIndex PluginListModel::parent(const QModelIndex& idx) const
{
	// flat list, no parent
	return {};
}

int PluginListModel::rowCount(const QModelIndex& parent) const
{
    return m_plugins.size();
}

QVariant PluginListModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid()) return {};
    IPlugin* n = static_cast<IPlugin*>(idx.internalPointer());
	if (!n) return {};

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
			return n->name().c_str();
        }
        if (idx.column() == 1) { // version
            return QString::number(n->version());
        }
        if (idx.column() == 2) { // author
            // TODO: add author to plugin metadata
			return QString("Riley Meyerkorth"); //n->author().c_str();
        }
        if (idx.column() == 3) { // type
            return QString::fromStdString(pluginTypeToString(n->type()));
        }
        if (idx.column() == 4) { // isCore
			// TODO: add isCore to plugin metadata
            return QString("Yes"); //n->isCore() ? QString("Yes") : QString("No");
        }
    }
    if (role == Qt::UserRole) {
        return QVariant::fromValue(n);
    }
    return {};
}

void PluginListModel::buildFlat()
{
    for (auto& plugin : m_pluginController.plugins()) {
        m_plugins.append(plugin);
    }
}

void PluginListModel::rebuild()
{
    beginResetModel();
    m_plugins.clear();
    this->buildFlat();
    endResetModel();
}