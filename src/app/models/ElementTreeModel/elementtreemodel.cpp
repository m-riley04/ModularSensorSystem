#include "elementtreemodel.h"

ElementTreeModel::ElementTreeModel(MainController& mc,
    QObject* parent)
	: QAbstractItemModel(parent), m_mainController(mc)
{
	// Connect signals for rebuilds
    connect(&m_mainController.sourceController(), &SourceController::sourceAdded, this, &ElementTreeModel::onSourceAdded);
    connect(&m_mainController.sourceController(), &SourceController::sourceRemoved, this, &ElementTreeModel::removeNode);

    connect(&m_mainController.mountController(), &MountController::mountAdded, this, &ElementTreeModel::onMountAdded);
    connect(&m_mainController.mountController(), &MountController::mountRemoved, this, &ElementTreeModel::removeNode);

    connect(&m_mainController.processingController(), &ProcessingController::processorAdded, this, &ElementTreeModel::onProcessorAdded);
    connect(&m_mainController.processingController(), &ProcessingController::processorRemoved, this, &ElementTreeModel::removeNode);
}

ElementTreeModel::~ElementTreeModel()
{}

QModelIndex ElementTreeModel::index(int row, int col, const QModelIndex & p) const
{
    if (col < 0 || col >= columnCount(p)) return {};
    ElementTreeNode* parentNode = p.isValid()
        ? static_cast<ElementTreeNode*>(p.internalPointer())
        : nullptr;
    const auto& list = parentNode ? parentNode->children : m_rootNodes;
    if (row < 0 || row >= list.size()) return {};
    return createIndex(row, col, list[row]);
}

QModelIndex ElementTreeModel::parent(const QModelIndex& idx) const
{
    if (!idx.isValid()) return {};
    ElementTreeNode* node = static_cast<ElementTreeNode*>(idx.internalPointer());
    ElementTreeNode* parent = node->parent;
    if (!parent) return {};
    QList<ElementTreeNode*> siblings = parent->parent ? parent->parent->children : m_rootNodes;
    int row = std::find(siblings.begin(), siblings.end(), parent) - siblings.begin();
    return createIndex(row, 0, parent);
}

int ElementTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) return m_rootNodes.size();
    ElementTreeNode* node = static_cast<ElementTreeNode*>(parent.internalPointer());
    return node->children.size();
}

QVariant ElementTreeModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid()) return {};
    ElementTreeNode* n = static_cast<ElementTreeNode*>(idx.internalPointer());
	if (!n) return {};

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
            switch (n->kind) {
            case IElement::Type::Mount: {
                const Mount* m = m_mainController.mountController().byId(n->uuid);
                return m ? QString::fromStdString(m->name()) : QString("<unknown mount>");
            }
            case IElement::Type::Source: {
				const Source* s = m_mainController.sourceController().byId(n->uuid);
                return s ? QString::fromStdString(s->name()) : QString("<unknown source>");
            }
            case IElement::Type::Processor: {
                const Processor* p = m_mainController.processingController().byId(n->uuid);
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

void ElementTreeModel::removeNode(QUuid uuid)
{
    m_nodeMap.remove(uuid);
    m_rootNodes.erase(std::remove_if(m_rootNodes.begin(), m_rootNodes.end(),
        [uuid](ElementTreeNode* n) { return n->uuid == uuid; }),
		m_rootNodes.end());

    //// Recursive removal that preserves access to the node before erasing it from the hash.
    //std::function<void(QUuid)> removeRec = [&](const QUuid& id) {
    //    ElementTreeNode* node = m_nodeMap.contains(id) ? &m_nodeMap[id] : nullptr;
    //    if (!node) return;
    //    // Copy children list to avoid mutation during iteration
    //    const QList<ElementTreeNode*> children = node->children;
    //    for (ElementTreeNode* child : children) {
    //        removeRec(child->uuid);
    //    }
    //    // Detach from parent/root
    //    if (node->parent) {
    //        node->parent->children.removeAll(node);
    //    }
    //    else {
    //        m_rootNodes.removeAll(node);
    //    }
    //    // Finally remove this node
    //    m_nodeMap.remove(id);
    //    };

    //beginResetModel(); // Simplify view update (could be optimized with beginRemoveRows per node)
    //removeRec(uuid);
    //endResetModel();
}

void ElementTreeModel::onMountAdded(Mount* m)
{
    QUuid mId = boostUuidToQUuid(m->uuid());
    if (!m_nodeMap.contains(mId)) {
        // TODO: optimize to only add the new mount node
        this->rebuild();
    }
}

void ElementTreeModel::onSourceAdded(Source* s)
{
    QUuid sId = boostUuidToQUuid(s->uuid());
    if (!m_nodeMap.contains(sId)) {
        // TODO: optimize to only add the new source node
        this->rebuild();
    }
}

void ElementTreeModel::onProcessorAdded(Processor* p)
{
    QUuid pId = boostUuidToQUuid(p->uuid());
    if (!m_nodeMap.contains(pId)) {
        // TODO: optimize to only add the new processor node
        this->rebuild();
    }
}

void ElementTreeModel::buildFlat()
{
    for (Mount* m : m_mainController.mountController().mounts()) {
        QUuid mId = boostUuidToQUuid(m->uuid());
        m_nodeMap.insert(mId, ElementTreeNode{
            .uuid = mId,
            .kind = IElement::Type::Mount,
            .parent = nullptr,
            .children = {}
        });
		m_rootNodes.append(&m_nodeMap[mId]);
    }

    for (auto s : m_mainController.sourceController().sources()) {
		QUuid sId = boostUuidToQUuid(s->uuid());
        m_nodeMap.insert(sId, ElementTreeNode{
            .uuid = sId,
            .kind = IElement::Type::Source,
            .parent = nullptr,
            .children = {}
        });
		m_rootNodes.append(&m_nodeMap[sId]);
    }

    for (auto p : m_mainController.processingController().processors()) {
        QUuid pId = boostUuidToQUuid(p->uuid());
		m_nodeMap.insert(pId, ElementTreeNode{
            .uuid = pId,
            .kind = IElement::Type::Processor,
            .parent = nullptr,
            .children = {}
            });
		m_rootNodes.append(&m_nodeMap[pId]);
    }
}

void ElementTreeModel::buildHierarchical()
{
    for (Mount* m : m_mainController.mountController().mounts()) {
        QUuid mId = boostUuidToQUuid(m->uuid());
        m_nodeMap.insert(mId, ElementTreeNode{
            .uuid = mId,
            .kind = IElement::Type::Mount,
            .parent = nullptr,
            .children = {}
        });
        ElementTreeNode* mountNode = &m_nodeMap[mId];
        m_rootNodes.append(mountNode);

        for (auto s : m_mainController.sessionController().getSourcesByMount(mId)) {
            QUuid sId = boostUuidToQUuid(s->uuid());
            m_nodeMap.insert(sId, ElementTreeNode{
                .uuid = sId,
                .kind = IElement::Type::Source,
                .parent = mountNode,
                .children = {}
            });
            ElementTreeNode* sourceNode = &m_nodeMap[sId];
            mountNode->children.append(sourceNode);

            for (auto p : m_mainController.sessionController().getProcessorsBySource(sId)) {
                QUuid pId = boostUuidToQUuid(p->uuid());
                m_nodeMap.insert(pId, ElementTreeNode{
                    .uuid = pId,
                    .kind = IElement::Type::Processor,
                    .parent = sourceNode,
                    .children = {}
                });
                ElementTreeNode* procNode = &m_nodeMap[pId];
                sourceNode->children.append(procNode);
            }
        }
    }
}

void ElementTreeModel::rebuild(bool isFlat)
{
    beginResetModel();
    m_nodeMap.clear();
	m_rootNodes.clear();

    if (isFlat) {
		this->buildFlat();
    }
    else {
        this->buildHierarchical();
    }
    endResetModel();
}