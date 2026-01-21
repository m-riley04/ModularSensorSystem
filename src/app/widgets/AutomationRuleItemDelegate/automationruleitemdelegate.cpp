#include "automationruleitemdelegate.h"
#include <widgets/AutomationRuleItemWidget/automationrulelistitemwidget.h>

// Some of these implementations were structured from Qt docs: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

QWidget* AutomationRuleItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarEditor* editor = new StarEditor(parent);
        connect(editor, &StarEditor::editingFinished,
            this, &StarDelegate::commitAndCloseEditor);
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void AutomationRuleItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarRating starRating = qvariant_cast<StarRating>(index.data());
        StarEditor* starEditor = qobject_cast<StarEditor*>(editor);
        starEditor->setStarRating(starRating);
    }
    else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void AutomationRuleItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarEditor* starEditor = qobject_cast<StarEditor*>(editor);
        model->setData(index, QVariant::fromValue(starEditor->starRating()));
    }
    else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

QSize AutomationRuleItemDelegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarRating starRating = qvariant_cast<StarRating>(index.data());
        return starRating.sizeHint();
    }
    return QStyledItemDelegate::sizeHint(option, index);
}