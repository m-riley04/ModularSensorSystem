#pragma once

#include <QAbstractListModel>
#include <controllers/rulescontroller.hpp>
#include <models/rule_models.hpp>
#include <vector>

class AutomationRulesListModel : public QAbstractListModel
{
public:
	enum Roles {
		RuleRole = Qt::UserRole + 1,
		ActionTypeRole,
		ActionTargetRole,
		TriggerTypeRole,
		TriggerConditionRole,
	};

	explicit AutomationRulesListModel(RulesController& rc, QObject* parent = nullptr);
	~AutomationRulesListModel() override = default;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	Rule ruleAt(int row) const;
	void rebuild();

private:
	RulesController& m_rulesController;
	std::vector<Rule> m_rules;
};

Q_DECLARE_METATYPE(AutomationRulesListModel*)
