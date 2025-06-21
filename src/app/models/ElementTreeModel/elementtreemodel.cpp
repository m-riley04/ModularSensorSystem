#include "elementtreemodel.h"

ElementTreeModel::ElementTreeModel(MountController* mc,
    SourceController* sc,
    ProcessingController* pc,
    QObject* parent)
	: QAbstractItemModel(parent)
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
            case Node::Kind::Mount: return mMountController->byId(n.id)->name();
            case Node::Kind::Source: return mSourceController->byId(n.id)->name();
            //case Node::Kind::Processor: return mProcessingController->byId(n.id)->name();
            }
        }
        if (idx.column() == 1) { /* status text */ }
    }
    return {};
}

void ElementTreeModel::rebuild()
{
    beginResetModel();
    mNodes.clear();

    // flat list with parent indices
    for (Mount* m : mMountController->mounts()) {
        int mRow = mNodes.size();
        mNodes << Node{ Node::Kind::Mount, QUuid(m->id()), -1};

        /*for (auto sId : mMountController->sourcesOf(m->id())) {
            int sRow = mNodes.size();
            mNodes << Node{ Node::Kind::Source, sId, mRow };

            for (auto pId : mProcessingController->processorsForSource(sId)) {
                mNodes << Node{ Node::Kind::Processor, pId, sRow };
            }
        }*/
    }
    endResetModel();
}