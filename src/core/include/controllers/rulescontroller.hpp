#pragma once

#include <QObject>
#include "models/rule_models.hpp"
#include <automation/automation_event.hpp>
#include <vector>
#include <controllers/elementscontroller.hpp>
#include <map>
#include <controllers/sessioncontroller.hpp>

/**
 * @brief A controller class for managing rules within the application.
 * Rules are for automating actions based on processor events.
 */
class RulesController : public QObject
{
private:
	ElementsController& m_elementsController;
	SessionController& m_sessionController;

	std::vector<Rule> m_rules;

	void onAutomationEvent(const AutomationEvent& event);

	/**
	 * @brief Checks all active rules to see if their trigger conditions are met. Executes associated actions if conditions are met.
	 */
	void checkRules();

	/**
	 * @brief Checks if a rule trigger condition is met.
	 * @param trigger The rule trigger to check.
	 * @return True if the trigger condition is met, false otherwise.
	 */
	bool checkRuleTrigger(const RuleTrigger& trigger);

	/**
	 * @brief Executes the action associated with a rule.
	 * @param action The rule action to execute.
	 */
	void executeRuleAction(const RuleAction& action);

public:
	explicit RulesController(SessionController& sc, ElementsController& ec, QObject* parent);
	~RulesController();

	const std::vector<Rule>& rules() const { return m_rules; }

	void addRule(const Rule& rule) { m_rules.push_back(rule); }
	bool updateRule(int index, const Rule& rule)
	{
		if (index < 0 || index >= static_cast<int>(m_rules.size())) return false;
		m_rules[static_cast<size_t>(index)] = rule;
		return true;
	}
	bool removeRule(int index)
	{
		if (index < 0 || index >= static_cast<int>(m_rules.size())) return false;
		m_rules.erase(m_rules.begin() + index);
		return true;
	}
};