#pragma once

#include <QObject>
#include "models/rule_models.hpp"
#include <vector>
#include <controllers/elementscontroller.hpp>
#include <map>

/**
 * @brief A controller class for managing rules within the application.
 * Rules are for automating actions based on processor events.
 */
class RulesController : public QObject
{
	Q_OBJECT
private:
	ElementsController& m_elementsController;

	std::vector<Rule> m_rules;

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
	explicit RulesController(ElementsController&, QObject*);
	~RulesController();

	void addRule(const Rule rule);
};