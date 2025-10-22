#include "elementtreemodel.h"
#include "controllers/datapipelinecontroller.h"

ElementTreeModel::ElementTreeModel(MainController* mc,
    QObject* parent)
	: QAbstractItemModel(parent), m_mainController(mc)
{}

ElementTreeModel::~ElementTreeModel()
{}

QModelIndex ElementTreeModel::index(int row, int col, const QModelIndex & p) const
{
    int parentRow = p.isValid() ? p.internalId() : -1;
    // find Nth child with that parentRow
    int count = -1;
    for (int i = 0; i < mNodes.size(); ++i) {
        if (mNodes[i].parentIndex == parentRow) {
            if (++count == row)
                return createIndex(row, col, quintptr(i)); // store node index
        }
    }
    return {};
}

QModelIndex ElementTreeModel::parent(const QModelIndex& idx) const
{
    if (!idx.isValid()) return {};
    int nodeIdx = int(idx.internalId());
    int parentIdx = mNodes[nodeIdx].parentIndex;
    if (parentIdx < 0) return {};

    // find row of the parent within its own parent
    int grandParent = mNodes[parentIdx].parentIndex;
    int row = 0;
    for (int i = 0; i < parentIdx; ++i) {
        if (mNodes[i].parentIndex == grandParent) ++row;
    }

    return createIndex(row, 0, quintptr(parentIdx));
}

int ElementTreeModel::rowCount(const QModelIndex& p) const
{
    int parentRow = p.isValid() ? p.internalId() : -1;
    return std::count_if(mNodes.begin(), mNodes.end(),
        [=](const Node& n) { return n.parentIndex == parentRow; });
}

QVariant ElementTreeModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid()) return {};
    const Node& n = mNodes[int(idx.internalId())];

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
            switch (n.kind) {
            case Node::Kind::Mount: {
                const Mount* m = m_mainController->mountController()->byId(n.id);
                return m ? QString::fromStdString(m->name()) : QString("<unknown mount>");
            }
            case Node::Kind::Source: return m_mainController->sourceController()->byId(n.id)->name();
            case Node::Kind::Processor: return QString("Processor");
            default: break;
            }
        }
        if (idx.column() == 1) { /* status text */ }
    }
    if (role == Qt::UserRole) {
		return QVariant::fromValue(n);
    }
    return {};
}

void ElementTreeModel::buildFlat()
{
    for (Mount* m : m_mainController->mountController()->mounts()) {
        // Use controller-assigned ID
        QUuid id = m_mainController->mountController()->idFor(m);
        mNodes << Node{ Node::Kind::Mount, id, -1 };
    }

    for (auto s : m_mainController->sourceController()->sources()) {
        mNodes << Node{ Node::Kind::Source, s->id(), -1 };
    }

    for (auto p : m_mainController->processingController()->processors()) {
        // TODO: assign proper processor UUID
        mNodes << Node{ Node::Kind::Processor, QUuid::createUuid(), -1 };
    }
}

void ElementTreeModel::buildHierarchical()
{
    // TODO: add this better hierarchical structure instead of simple list of elements
    // flat list with parent indices
    for (Mount* m : m_mainController->mountController()->mounts()) {
        int mRow = mNodes.size();
        // Use controller-assigned ID
        QUuid id = m_mainController->mountController()->idFor(m);
        mNodes << Node{ Node::Kind::Mount, id, -1 };

        for (auto s : m_mainController->dataPipelineController()->getSourcesByMount(id)) {
            int sRow = mNodes.size();
            mNodes << Node{ Node::Kind::Source, s->id(), mRow };

            for (auto p : m_mainController->dataPipelineController()->getProcessorsBySource(s->id())) {
                // TODO: use real processor ID
                mNodes << Node{ Node::Kind::Processor, QUuid::createUuid(), sRow };
            }
        }
    }
}

void ElementTreeModel::rebuild(bool isFlat)
{
    beginResetModel();
    mNodes.clear();

    if (isFlat) {
		this->buildFlat();
    }
    else {
        this->buildHierarchical();
    }
    endResetModel();
}