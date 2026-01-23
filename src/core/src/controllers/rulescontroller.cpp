#include <controllers/rulescontroller.hpp>
#include <controllers/loggingcontroller.hpp>

#include <models/rule_models.hpp>

#include <QString>

RulesController::RulesController(SessionController& sc, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	connect(&m_sessionController, &SessionController::automationEvent,
		this, &RulesController::onAutomationEvent, Qt::QueuedConnection);
}

RulesController::~RulesController()
{

}

void RulesController::checkRules()
{
	LoggingController::debug("Checking all rules...");

	for (const Rule& rule : m_rules) {
		if (!rule.isActive()) continue;
		if (!checkRuleTrigger(rule.trigger())) continue;

		// If we reach here, trigger the action
		executeRuleAction(rule.action());
	}
}

void RulesController::onAutomationEvent(const AutomationEvent& event)
{
	// Basic skeleton: match by triggerType only (condition evaluation can be added later)
	for (const Rule& rule : m_rules) {
		if (!rule.isActive()) continue;

		if (rule.trigger().triggerType() != RuleTriggerType::AutomationEventType) continue;
		if (event.type != "automation.event") continue;

		executeRuleAction(rule.action());
	}
}

bool RulesController::checkRuleTrigger(const RuleTrigger& trigger)
{
	LoggingController::debug("Checking trigger for rule #" + QString::number(trigger.ruleId())); // TODO/CONSIDER: should logging be here? Might get spammy.

	// TODO: Placeholder for checking the trigger condition
	// e.g., evaluate condition based on trigger.triggerType and trigger.condition

	return false;
}

void RulesController::executeRuleAction(const RuleAction& action)
{
	LoggingController::debug("Executing action for rule #" + QString::number(action.ruleId()));

	// TODO: Placeholder for executing the action specified in the rule
	// e.g., perform action based on action.actionType and action.target

	switch (action.actionType()) {
		case RuleActionType::SessionStartRecording:
			m_sessionController.startRecording();
			return;
		case RuleActionType::SessionStopRecording:
			m_sessionController.stopRecording();
			return;
		case RuleActionType::SessionStartProcessing:
			m_sessionController.startProcessing();
			return;
		case RuleActionType::SessionStopProcessing:
			m_sessionController.stopProcessing();
			return;
		default:
			return;
	}
}
