#include "elementtreemodel.h"
#include "controllers/sessioncontroller.h"
#include <utils/boost_qt_conversions.h>

ElementTreeModel::ElementTreeModel(MainController* mc,
    QObject* parent)
	: QAbstractItemModel(parent), m_mainController(mc)
{
	// Connect signals for rebuilds
    connect(m_mainController->sourceController(), &SourceController::sourceAdded, this, [this]() {
        this->rebuild();
		});

    connect(m_mainController->mountController(), &MountController::mountAdded, this, &ElementTreeModel::rebuild);

    connect(m_mainController->processingController(), &ProcessingController::processorAdded, this, &ElementTreeModel::rebuild);

    connect(m_mainController->mountController(), &MountController::mountRemoved, this, &ElementTreeModel::removeNode);
}

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
        [=](const ElementTreeNode& n) { return n.parentIndex == parentRow; });
}

QVariant ElementTreeModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid()) return {};
    const ElementTreeNode& n = mNodes[int(idx.internalId())];

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
            switch (n.kind) {
            case ElementTreeNode::Kind::Mount: {
                const Mount* m = m_mainController->mountController()->byId(n.id);
                return m ? QString::fromStdString(m->name()) : QString("<unknown mount>");
            }
            case ElementTreeNode::Kind::Source: return QString::fromStdString(m_mainController->sourceController()->byId(n.id)->name());
            case ElementTreeNode::Kind::Processor: return QString("Processor");
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

ElementTreeNode* ElementTreeModel::findNode(QUuid uuid)
{
    for (auto& node : mNodes) {
        if (node.id == uuid) {
            return &node;
        }
    }
}

void ElementTreeModel::removeNode(QUuid uuid)
{
    // Removes a node at a specific UUID
    mNodes.removeIf([this, uuid](ElementTreeNode node) {
        return node.id == uuid;
        });
}

void ElementTreeModel::buildFlat()
{
    for (Mount* m : m_mainController->mountController()->mounts()) {
        // Use controller-assigned ID
        QUuid mId = boostUuidToQUuid(m->uuid());
        mNodes << ElementTreeNode{ ElementTreeNode::Kind::Mount, mId, -1 };
    }

    for (auto s : m_mainController->sourceController()->sources()) {
		// Use universal uuid
		QUuid sId = boostUuidToQUuid(s->uuid());
        mNodes << ElementTreeNode{ ElementTreeNode::Kind::Source, sId, -1 };
    }

    for (auto p : m_mainController->processingController()->processors()) {
		QUuid pId = boostUuidToQUuid(p->uuid());
        mNodes << ElementTreeNode{ ElementTreeNode::Kind::Processor, pId, -1 };
    }
}

void ElementTreeModel::buildHierarchical()
{
    // TODO: add this better hierarchical structure instead of simple list of elements
    // flat list with parent indices
    for (Mount* m : m_mainController->mountController()->mounts()) {
        int mRow = mNodes.size();
        // Use controller-assigned ID
        QUuid id = boostUuidToQUuid(m->uuid());
        mNodes << ElementTreeNode{ ElementTreeNode::Kind::Mount, id, -1};

        for (auto s : m_mainController->sessionController()->getSourcesByMount(id)) {
            int sRow = mNodes.size();
			QUuid sId = boostUuidToQUuid(s->uuid());
            mNodes << ElementTreeNode{ ElementTreeNode::Kind::Source, sId, mRow };

            for (auto p : m_mainController->sessionController()->getProcessorsBySource(sId)) {
                // TODO: use real processor ID
                mNodes << ElementTreeNode{ ElementTreeNode::Kind::Processor, boostUuidToQUuid(p->uuid()), sRow};
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