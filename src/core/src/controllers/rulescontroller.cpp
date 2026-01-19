#include <controllers/rulescontroller.hpp>
#include <controllers/loggingcontroller.hpp>

RulesController::RulesController(ElementsController& ec, QObject* parent)
	: QObject(parent), m_elementsController(ec)
{

}

RulesController::~RulesController()
{

}

void RulesController::checkRules()
{
	LoggingController::debug("Checking all rules...");

	for (const Rule& rule : m_rules) {
		if (!rule.isActive) continue;
		if (!checkRuleTrigger(rule.trigger)) continue;

		// If we reach here, trigger the action
		executeRuleAction(rule.action);
	}
}

bool RulesController::checkRuleTrigger(const RuleTrigger& trigger)
{
	LoggingController::debug("Checking trigger for rule #" + QString::number(trigger.ruleId)); // TODO/CONSIDER: should logging be here? Might get spammy.

	// TODO: Placeholder for checking the trigger condition
	// e.g., evaluate condition based on trigger.triggerType and trigger.condition

	return false;
}

void RulesController::executeRuleAction(const RuleAction& action)
{
	LoggingController::debug("Executing action for rule #" + QString::number(action.ruleId));

	// TODO: Placeholder for executing the action specified in the rule
	// e.g., perform action based on action.actionType and action.target
}

void RulesController::addRule(const Rule rule)
{
	m_rules.push_back(rule);
}
