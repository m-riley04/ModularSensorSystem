#pragma once

#include <QAbstractItemModel>
#include <controllers/MountController/mountcontroller.h>
#include <controllers/SourceController/sourcecontroller.h>
#include <controllers/ProcessingController/processingcontroller.h>
#include <features/mounts/Mount/mount.h>

struct Node {
    enum class Kind { Mount, Source, Processor };
    Kind kind;
    QUuid id;                  // the controller's primary key
    int parentIndex;         // index in m_nodes (-1 for root)
};

class ElementTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ElementTreeModel(MountController* mc, SourceController* sc, ProcessingController* pc, QObject* parent = nullptr);
	~ElementTreeModel();

    // --- mandatory overrides ---
    QModelIndex index(int row, int col, const QModelIndex& p) const override;
    QModelIndex parent(const QModelIndex& c)  const override;
    int rowCount(const QModelIndex& p) const override;
    int columnCount(const QModelIndex&)   const override { return 2; }
    QVariant data(const QModelIndex&, int role) const override;

public slots:
    void rebuild();                          // quick & dirty first
    //void onMountAdded(Mount*);
    //void onSourceAdded(Source*);
    //void onProcessorAdded(ProcessorBase*);

private:
    QVector<Node> mNodes;
    MountController* mMountController;
    SourceController* mSourceController;
    ProcessingController* mProcessingController;

    //static QVariant iconFor(Node::Kind k);
};

