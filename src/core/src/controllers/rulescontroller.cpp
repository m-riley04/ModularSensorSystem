#include <controllers/rulescontroller.hpp>
#include <controllers/loggingcontroller.hpp>

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

	for (const RuleModel& rule : m_rules) {
		if (!rule.isActive) continue;
		if (!checkRuleTrigger(rule.trigger)) continue;

		// If we reach here, trigger the action
		executeRuleAction(rule.action);
	}
}

void RulesController::onAutomationEvent(const AutomationEvent& event)
{
	// Basic skeleton: match by triggerType only (condition evaluation can be added later)
	for (const RuleModel& rule : m_rules) {
		if (!rule.isActive) continue;

		const QString triggerType = QString::fromUtf8(rule.trigger.triggerType.data(),
			static_cast<int>(rule.trigger.triggerType.size()));
		if (triggerType != event.type) continue;

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

	const QString actionType = QString::fromUtf8(action.actionType.data(),
		static_cast<int>(action.actionType.size()));

	if (actionType == "session.startRecording") {
		m_sessionController.startRecording();
		return;
	}
	if (actionType == "session.stopRecording") {
		m_sessionController.stopRecording();
		return;
	}
	if (actionType == "session.startProcessing") {
		m_sessionController.startProcessing();
		return;
	}
	if (actionType == "session.stopProcessing") {
		m_sessionController.stopProcessing();
		return;
	}

	LoggingController::warning("Unknown rule actionType: " + actionType);
}

void RulesController::addRule(const RuleModel& rule)
{
	m_rules.push_back(rule);
}
