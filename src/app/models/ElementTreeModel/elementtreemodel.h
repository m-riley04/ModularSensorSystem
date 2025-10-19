#pragma once

#include <QAbstractItemModel>
#include "controllers/mountcontroller.h"
#include "controllers/sourcecontroller.h"
#include "controllers/processingcontroller.h"
#include "features/mounts/mount.h"
#include <controllers/maincontroller.h>

struct Node {
    enum class Kind { Mount, Source, Processor };
    Kind kind;
    QUuid id; // the controller's primary key
    int parentIndex; // index in m_nodes (-1 for root)
};

class ElementTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ElementTreeModel(MainController*, QObject* parent = nullptr);
	~ElementTreeModel();

    // --- mandatory overrides ---
    QModelIndex index(int row, int col, const QModelIndex& p) const override;
    QModelIndex parent(const QModelIndex& c)  const override;
    int rowCount(const QModelIndex& p) const override;
    int columnCount(const QModelIndex&)   const override { return 2; }
    QVariant data(const QModelIndex&, int role) const override;

public slots:
    void rebuild(bool isFlat = true); // quick & dirty first
	// TODO: incremental updates instead of complete rebuilds
    /*void onMountAdded(Mount*);
    void onSourceAdded(Source*);
    void onProcessorAdded(ProcessorBase*);*/

private:
    QVector<Node> mNodes;
    MainController* pMainController;

    //static QVariant iconFor(Node::Kind k);
};

