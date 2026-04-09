#include <controllers/rulescontroller.hpp>
#include <controllers/loggingcontroller.hpp>

#include <models/rule_models.hpp>

RulesController::RulesController(SessionController& sc, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	connect(&m_sessionController, &SessionController::automationEvent,
		this, &RulesController::onAutomationEvent, Qt::QueuedConnection);

	// Register built-in actions
	registerAction(AutomationActionStrings::SessionStartRecording, [&](const RuleAction&) {
		m_sessionController.startRecording();
	});
	registerAction(AutomationActionStrings::SessionStopRecording, [&](const RuleAction&) {
		m_sessionController.stopRecording();
	});
	registerAction(AutomationActionStrings::SessionStartProcessing, [&](const RuleAction&) {
		m_sessionController.startProcessing();
	});
	registerAction(AutomationActionStrings::SessionStopProcessing, [&](const RuleAction&) {
		m_sessionController.stopProcessing();
	});
}

RulesController::~RulesController()
{

}

void RulesController::onAutomationEvent(const AutomationEvent& event)
{
	for (const Rule& rule : m_rules) {
		if (!rule.isActive()) continue;
		if (rule.trigger().eventType() != event.type) continue;

		// TODO: evaluate rule.trigger().condition() against event.payload
		executeRuleAction(rule.action());
	}
}

bool RulesController::updateRule(int index, const Rule& rule)
{
	if (index < 0 || index >= static_cast<int>(m_rules.size()))
		return false;

	m_rules[index] = rule;
	return true;
}

bool RulesController::removeRule(int index)
{
	if (index < 0 || index >= static_cast<int>(m_rules.size()))
		return false;

	m_rules.erase(m_rules.begin() + index);
	return true;
}

void RulesController::registerAction(const QString& actionType, ActionHandler handler)
{
	m_actionHandlers[actionType] = std::move(handler);
}

void RulesController::executeRuleAction(const RuleAction& action)
{
	auto it = m_actionHandlers.find(action.actionType());
	if (it != m_actionHandlers.end()) {
		it->second(action);
	} else {
		LoggingController::warning("No handler registered for action: "
			+ action.actionType());
	}
}
