#include "elementtreemodel.h"

ElementTreeModel::ElementTreeModel(MainController* mc,
    QObject* parent)
	: QAbstractItemModel(parent), m_mainController(mc)
{
	// Connect signals for rebuilds
    connect(&m_mainController->sourceController(), &SourceController::sourceAdded, this, [this]() {
        this->rebuild();
		});

    connect(&m_mainController->mountController(), &MountController::mountAdded, this, &ElementTreeModel::rebuild);

    connect(&m_mainController->processingController(), &ProcessingController::processorAdded, this, &ElementTreeModel::rebuild);

    connect(&m_mainController->mountController(), &MountController::mountRemoved, this, &ElementTreeModel::removeNode);
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
            case IElement::Type::Mount: {
                const Mount* m = m_mainController->mountController().byId(n.uuid);
                return m ? QString::fromStdString(m->name()) : QString("<unknown mount>");
            }
            case IElement::Type::Source: {
				const Source* s = m_mainController->sourceController().byId(n.uuid);
                return s ? QString::fromStdString(s->name()) : QString("<unknown source>");
            }
            case IElement::Type::Processor: {
                const Processor* p = m_mainController->processingController().byId(n.uuid);
                return p ? QString::fromStdString(p->name()) : QString("<unknown processor>");
            }
            default: break;
            }
        }
        if (idx.column() == 1) { 
            return QString("Status not available");
        }
    }
    if (role == Qt::UserRole) {
		return QVariant::fromValue(n);
    }
    return {};
}

ElementTreeNode* ElementTreeModel::findNode(QUuid uuid)
{
    for (auto& node : mNodes) {
        if (node.uuid == uuid) {
            return &node;
        }
    }
}

void ElementTreeModel::removeNode(QUuid uuid)
{
    // Removes a node at a specific UUID
    mNodes.removeIf([this, uuid](ElementTreeNode node) {
        return node.uuid == uuid;
        });
}

void ElementTreeModel::buildFlat()
{
    for (Mount* m : m_mainController->mountController().mounts()) {
        // Use controller-assigned ID
        QUuid mId = boostUuidToQUuid(m->uuid());
        mNodes << ElementTreeNode{
            .uuid = mId,
            .parentIndex = -1,
            .kind = IElement::Type::Mount
        };
    }

    for (auto s : m_mainController->sourceController().sources()) {
		// Use universal uuid
		QUuid sId = boostUuidToQUuid(s->uuid());
        mNodes << ElementTreeNode{
            .uuid = sId,
            .parentIndex = -1,
            .kind = IElement::Type::Source
        };
    }

    for (auto p : m_mainController->processingController().processors()) {
        QUuid pId = boostUuidToQUuid(p->uuid());
        mNodes << ElementTreeNode{
            .uuid = pId,
            .parentIndex = -1,
            .kind = IElement::Type::Processor
        };
    }
}

void ElementTreeModel::buildHierarchical()
{
    // TODO: add this better hierarchical structure instead of simple list of elements
    // flat list with parent indices
    for (Mount* m : m_mainController->mountController().mounts()) {
        int mRow = mNodes.size();
        // Use controller-assigned ID
        QUuid uuid = boostUuidToQUuid(m->uuid());
        mNodes << ElementTreeNode{
            .uuid = uuid,
            .parentIndex = -1,
            .kind = IElement::Type::Mount
        };

        for (auto s : m_mainController->sessionController().getSourcesByMount(uuid)) {
            int sRow = mNodes.size();
			QUuid sId = boostUuidToQUuid(s->uuid());
            mNodes << ElementTreeNode{
                .uuid = sId,
                .parentIndex = mRow,
                .kind = IElement::Type::Source
            };

            for (auto p : m_mainController->sessionController().getProcessorsBySource(sId)) {
                // TODO: use real processor ID
                mNodes << ElementTreeNode{
                    .uuid = boostUuidToQUuid(p->uuid()),
                    .parentIndex = sRow,
                    .kind = IElement::Type::Processor
                };
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