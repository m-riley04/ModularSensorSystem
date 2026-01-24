#include "automationruleslistmodel.h"
#include <algorithm>

AutomationRulesListModel::AutomationRulesListModel(RulesController& rc, QObject* parent)
	: QAbstractListModel(parent)
	, m_rulesController(rc)
{
	rebuild();
}

int AutomationRulesListModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) return 0;
	return static_cast<int>(m_rules.size());
}

QVariant AutomationRulesListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) return {};
	if (index.row() < 0 || index.row() >= static_cast<int>(m_rules.size())) return {};

	const auto& r = m_rules[static_cast<size_t>(index.row())];

	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole: {
			const QString desc = QString::fromStdString(r.description());
			return desc.isEmpty() ? QString("(unnamed rule)") : desc;
		}
		case Roles::RuleRole:
			return QVariant::fromValue(r.description().c_str());
		case Roles::ActionTypeRole:
			return QString::fromStdString(toString(r.action().actionType()));
		case Roles::ActionTargetRole:
			return QString::fromStdString(r.action().target());
		case Roles::TriggerTypeRole:
			return QString::fromStdString(toString(r.trigger().triggerType()));
		case Roles::TriggerConditionRole:
			return QString::fromStdString(r.trigger().condition());
		case Qt::CheckStateRole:
			return r.isActive() ? Qt::Checked : Qt::Unchecked;
		default:
			return {};
	}

	return {};
}

Qt::ItemFlags AutomationRulesListModel::flags(const QModelIndex& index) const
{
	if (!index.isValid()) return Qt::NoItemFlags;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

bool AutomationRulesListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) return false;
	if (index.row() < 0 || index.row() >= static_cast<int>(m_rules.size())) return false;

	auto& r = m_rules[static_cast<size_t>(index.row())];

	switch (role) {
		case Qt::EditRole:
			r.setDescription(value.toString().toStdString());
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
			return true;
		case Roles::ActionTargetRole:
			r.action().setTarget(value.toString().toStdString());
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Roles::ActionTargetRole });
			return true;
		case Roles::ActionTypeRole:
			{
				RuleActionType t{};
				if (!tryParseRuleActionType(value.toString().toStdString(), t)) return false;
				r.action().setActionType(t);
			}
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Roles::ActionTypeRole });
			return true;
		case Roles::TriggerConditionRole:
			r.trigger().setCondition(value.toString().toStdString());
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Roles::TriggerConditionRole });
			return true;
		case Roles::TriggerTypeRole:
			{
				RuleTriggerType t{};
				if (!tryParseRuleTriggerType(value.toString().toStdString(), t)) return false;
				r.trigger().setTriggerType(t);
			}
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Roles::TriggerTypeRole });
			return true;
		case Qt::CheckStateRole:
			r.setActive(value.toInt() == Qt::Checked);
			m_rulesController.updateRule(index.row(), r);
			emit dataChanged(index, index, { Qt::CheckStateRole });
			return true;
		default:
			return false;
	}

	return false;
}

bool AutomationRulesListModel::insertRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false;
	if (count <= 0) return false;
	row = std::clamp(row, 0, static_cast<int>(m_rules.size()));

	beginInsertRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		RuleTrigger trig(-1, RuleTriggerType::AutomationEventType, "");
		RuleAction act(-1, RuleActionType::SessionStartRecording, "");
		Rule r(-1, "New rule", true, trig, act);
		m_rulesController.addRule(r);
		m_rules.insert(m_rules.begin() + (row + i), r);
	}
	endInsertRows();

	return true;
}

bool AutomationRulesListModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (parent.isValid()) return false;
	if (count <= 0) return false;
	if (row < 0 || row >= static_cast<int>(m_rules.size())) return false;

	const int last = std::min(row + count - 1, static_cast<int>(m_rules.size()) - 1);
	beginRemoveRows(QModelIndex(), row, last);
	for (int i = last; i >= row; --i) {
		m_rulesController.removeRule(i);
	}
	m_rules.erase(m_rules.begin() + row, m_rules.begin() + last + 1);
	endRemoveRows();
	return true;
}

Rule AutomationRulesListModel::ruleAt(int row) const
{
	if (row < 0 || row >= static_cast<int>(m_rules.size())) return {};
	return m_rules[static_cast<size_t>(row)];
}

void AutomationRulesListModel::rebuild()
{
	beginResetModel();
	m_rules = m_rulesController.rules();
	endResetModel();
}
