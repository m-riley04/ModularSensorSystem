#include "elementtreemodel.h"
#include "controllers/datapipelinecontroller.h"

ElementTreeModel::ElementTreeModel(MainController* mc,
    QObject* parent)
	: QAbstractItemModel(parent), pMainController(mc)
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

    /*if (role == TreeRoles::TypeRole) return int(n.kind);
    if (role == TreeRoles::IdRole)   return n.id;*/

    /*if (role == Qt::DecorationRole && idx.column() == 0)
        return iconFor(n.kind);*/

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
            switch (n.kind) {
            case Node::Kind::Mount: return QString::fromStdString(pMainController->mountController()->byId(n.id)->name());
            case Node::Kind::Source: return pMainController->sourceController()->byId(n.id)->name();
            //case Node::Kind::Processor: return QString(mProcessingController->byId(n.id)->name());
            }
        }
        if (idx.column() == 1) { /* status text */ }
    }
    return {};
}

void ElementTreeModel::rebuild(bool isFlat)
{
    beginResetModel();
    mNodes.clear();

    if (isFlat) {
        for (Mount* m : pMainController->mountController()->mounts()) {
            int mRow = mNodes.size();
			// TODO: use real mount ID
            mNodes << Node{ Node::Kind::Mount, QUuid(m->id()), -1 };
        }

        for (auto s : pMainController->sourceController()->sources()) {
            int sRow = mNodes.size();
            mNodes << Node{ Node::Kind::Source, s->id(), -1 };

        }

        for (auto p : pMainController->processingController()->processors()) {
            // TODO: assign proper processor UUID
            mNodes << Node{ Node::Kind::Processor, QUuid::createUuid(), -1 };
        }
    }
    else {
        // TODO: add this better hierarchical structure instead of simple list of elements
        // flat list with parent indices
        for (Mount* m : pMainController->mountController()->mounts()) {
            int mRow = mNodes.size();
            // TODO: use real mount ID
			QUuid id = QUuid(m->id());
            mNodes << Node{ Node::Kind::Mount, id, -1};

            for (auto s : pMainController->dataPipelineController()->getSourcesByMount(id)) {
                int sRow = mNodes.size();
                mNodes << Node{ Node::Kind::Source, s->id(), mRow};

                for (auto p : pMainController->dataPipelineController()->getProcessorsBySource(s->id())) {
					// TODO: use real processor ID
                    mNodes << Node{ Node::Kind::Processor, QUuid::createUuid(), sRow};
                }
            }
        }
    }
    endResetModel();
}