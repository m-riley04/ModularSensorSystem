#pragma once

#include <QVariant>
#include <QAbstractItemModel>
#include "controllers/mountcontroller.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "features/mounts/mount.hpp"
#include "controllers/maincontroller.hpp"
#include "controllers/sessioncontroller.hpp"
#include "utils/boost_qt_conversions.hpp"
#include "features/ielement.hpp"

struct ElementTreeNode {
    QUuid uuid; // the controller's primary key
    IElement::Type kind = IElement::Type::Unknown;
    ElementTreeNode* parent = nullptr;
	QList<ElementTreeNode*> children;
};

class ElementTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ElementTreeModel(MainController&, QObject* parent = nullptr);
	~ElementTreeModel();

    // --- mandatory overrides ---
    QModelIndex index(int row, int col, const QModelIndex& p) const override;
    QModelIndex parent(const QModelIndex& c)  const override;
    int rowCount(const QModelIndex& p) const override;
    int columnCount(const QModelIndex&)   const override { return 2; }
    QVariant data(const QModelIndex&, int role) const override;

public slots:
    void rebuild(bool isFlat = true);
    void removeNode(QUuid uuid);
    void onMountAdded(Mount*);
    void onSourceAdded(Source*);
    void onProcessorAdded(Processor*);

private:
    MainController& m_mainController;
	QHash<QUuid, ElementTreeNode> m_nodeMap;
	QList<ElementTreeNode*> m_rootNodes;

	void buildFlat();
	void buildHierarchical();

};

Q_DECLARE_METATYPE(ElementTreeModel*)