#include "pluginlistmodel.h"
#include <utils/element_utils.hpp>
#include <QBrush>
#include <QColor>

PluginListModel::PluginListModel(PluginController& c,
    QObject* parent)
    : QAbstractItemModel(parent), m_pluginController(c)
{
    this->rebuild(); // init built

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
    PluginMetadata* n = static_cast<PluginMetadata*>(idx.internalPointer());
	if (!n) return {};

    // Determine loaded state via registry
	bool isLoaded = m_pluginController.registry().isLoaded(n->path);

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
			return n->name.c_str();
        }
        if (idx.column() == 1) { // version
            return QString::number(n->version);
        }
        if (idx.column() == 2) { // author
            return QString::fromStdString(n->author);
        }
        if (idx.column() == 3) { // type
            return QString::fromStdString(elementTypeToString(n->type));
        }
        if (idx.column() == 4) { // isCore
            return n->isCore ? QString("Yes") : QString("No");
        }
    }
    if (role == Qt::UserRole) {
        return QVariant::fromValue(n);
    }
    if (role == Qt::CheckStateRole && idx.column() == 0) {
        // checked = loaded, unchecked = disabled
        return isLoaded ? Qt::Checked : Qt::Unchecked;
    }
    if (role == Qt::ForegroundRole) {
        // dim text if disabled
        if (!isLoaded) {
            return QBrush(QColor(Qt::gray));
        }
    }
    return {};
}

Qt::ItemFlags PluginListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == 0) {
        f |= Qt::ItemIsUserCheckable; // show checkbox for loaded state visual cue
    }
    return f;
}

void PluginListModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    std::sort(m_plugins.begin(), m_plugins.end(), [column, order](const PluginMetadata* a, const PluginMetadata* b) {
        int cmp = 0;
        switch (column) {
            case 0: // name
                cmp = QString::fromStdString(a->name).compare(QString::fromStdString(b->name), Qt::CaseInsensitive);
                break;
            case 1: // version
                cmp = static_cast<int>(a->version) - static_cast<int>(b->version);
                break;
            case 2: // author
                cmp = QString::fromStdString(a->author).compare(QString::fromStdString(b->author), Qt::CaseInsensitive);
                break;
            case 3: // type
                cmp = QString::fromStdString(elementTypeToString(a->type)).compare(QString::fromStdString(elementTypeToString(b->type)), Qt::CaseInsensitive);
                break;
            case 4: // isCore
                cmp = static_cast<int>(a->isCore) - static_cast<int>(b->isCore);
                break;
            default:
                break;
        }
        return (order == Qt::AscendingOrder) ? (cmp < 0) : (cmp > 0);
    });
	endResetModel();
}

QVariant PluginListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Name";
        case 1: return "Version";
        case 2: return "Author";
        case 3: return "Type";
        case 4: return "Core?";
        default: return QVariant();
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

void PluginListModel::buildFlat()
{
    for (auto& plugin : m_pluginController.registry().metadata()) {
        m_plugins.append(&plugin);
    }
}

void PluginListModel::rebuild()
{
    beginResetModel();
    m_plugins.clear();
    this->buildFlat();
    endResetModel();
}