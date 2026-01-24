#include "automationruleitemdelegate.h"
#include <widgets/AutomationRuleItemWidget/automationruleitemwidget.h>
#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>
#include <app/models/AutomationRulesListModel/automationruleslistmodel.h>

static Rule ruleFromIndex(const QModelIndex& index)
{
	auto* m = dynamic_cast<const AutomationRulesListModel*>(index.model());
	if (!m) return {};
	return m->ruleAt(index.row());
}

void AutomationRuleItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
}

// Some of these implementations were structured from Qt docs: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

QWidget* AutomationRuleItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	auto* editor = new AutomationRuleItemWidget(m_elementsController, m_sessionController, parent);

	connect(editor, &AutomationRuleItemWidget::editingFinished, this, [this, editor]() {
		auto* self = const_cast<AutomationRuleItemDelegate*>(this);
		emit self->commitData(editor);
		emit self->closeEditor(editor);
	});
	return editor;
}

void AutomationRuleItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	auto* w = qobject_cast<AutomationRuleItemWidget*>(editor);
	if (!w) {
		QStyledItemDelegate::setEditorData(editor, index);
		return;
	}

	w->setRule(ruleFromIndex(index));
}

void AutomationRuleItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const
{
	Q_UNUSED(editor);
	Q_UNUSED(model);
	Q_UNUSED(index);
}

QSize AutomationRuleItemDelegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}