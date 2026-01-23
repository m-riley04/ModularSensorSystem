#pragma once

#include <QStyledItemDelegate>
#include <QWidget>

class ElementsController;
class SessionController;

class AutomationRuleItemDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	AutomationRuleItemDelegate(ElementsController& ec, SessionController& sc, QObject* parent = nullptr)
		: QStyledItemDelegate(parent)
		, m_elementsController(ec)
		, m_sessionController(sc)
	{}

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
        const QModelIndex& index) const override;

private:
	ElementsController& m_elementsController;
	SessionController& m_sessionController;
};

