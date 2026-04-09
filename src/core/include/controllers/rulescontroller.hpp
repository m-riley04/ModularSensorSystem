#pragma once

#include <QObject>
#include "models/rule_models.hpp"
#include "core_export.hpp"
#include <automation/automation_event.hpp>
#include <vector>
#include <functional>
#include <unordered_map>
#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>

class MSS_CORE_API RulesController : public QObject
{
	Q_OBJECT

public:
	using ActionHandler = std::function<void(const RuleAction&)>;

	explicit RulesController(SessionController& sc, ElementsController& ec, QObject* parent);
	~RulesController();

	const std::vector<Rule>& rules() const { return m_rules; }

	void addRule(const Rule& rule) { m_rules.push_back(rule); }
	bool updateRule(int index, const Rule& rule);
	bool removeRule(int index);

	/// Register a named action handler (call from core or plugins)
	void registerAction(const QString& actionType, ActionHandler handler);

private:
	ElementsController& m_elementsController;
	SessionController& m_sessionController;

	std::vector<Rule> m_rules;
	std::unordered_map<QString, ActionHandler> m_actionHandlers;

	void onAutomationEvent(const AutomationEvent& event);
	void executeRuleAction(const RuleAction& action);
};