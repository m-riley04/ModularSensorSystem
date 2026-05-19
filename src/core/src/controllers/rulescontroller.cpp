#include <controllers/rulescontroller.hpp>
#include <controllers/loggingcontroller.hpp>

#include <models/rule_models.hpp>

void RulesController::registerBuiltInEventTypes() {
	registerEventType(AutomationEventStrings::PipelineStateChanged, tr("Pipeline state changed"));
	registerEventType(AutomationEventStrings::PipelineEos, tr("Pipeline EOS"));
	registerEventType(AutomationEventStrings::PipelineError, tr("Pipeline error"));
	registerEventType(AutomationEventStrings::RecordingStarted, tr("Recording started"));
	registerEventType(AutomationEventStrings::RecordingStopped, tr("Recording stopped"));
	registerEventType(AutomationEventStrings::ProcessorObjectDetected, tr("Object detected"));
	registerEventType(AutomationEventStrings::SessionStarted, tr("Session started"));
	registerEventType(AutomationEventStrings::SessionStopped, tr("Session stopped"));
	registerEventType(AutomationEventStrings::ProcessingStarted, tr("Processing started"));
	registerEventType(AutomationEventStrings::ProcessingStopped, tr("Processing stopped"));
}

void RulesController::registerBuiltInActions() {
	registerAction(AutomationActionStrings::SessionStartRecording,
		tr("Start recording"), [&](const RuleAction&) {
			m_sessionController.startRecording();
		});
	registerAction(AutomationActionStrings::SessionStopRecording,
		tr("Stop recording"), [&](const RuleAction&) {
			m_sessionController.stopRecording();
		});
	registerAction(AutomationActionStrings::SessionStartProcessing,
		tr("Start processing"), [&](const RuleAction&) {
			m_sessionController.startProcessing();
		});
	registerAction(AutomationActionStrings::SessionStopProcessing,
		tr("Stop processing"), [&](const RuleAction&) {
			m_sessionController.stopProcessing();
		});
}

RulesController::RulesController(SessionController& sc, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	connect(&m_sessionController, &SessionController::automationEvent,
		this, &RulesController::onAutomationEvent, Qt::QueuedConnection);

	// Register built-in event types and actions so they appear in the UI and have handlers
	registerBuiltInEventTypes();
	registerBuiltInActions();
	
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

void RulesController::registerEventType(const QString& id, const QString& displayName)
{
	// Avoid duplicates
	for (const auto& info : m_registeredEventTypes) {
		if (info.id == id) return;
	}
	m_registeredEventTypes.append({ id, displayName });
}

void RulesController::registerAction(const QString& actionType, const QString& displayName,
									 ActionHandler handler)
{
	// Always register the display entry (avoid duplicates)
	bool found = false;
	for (const auto& info : m_registeredActionTypes) {
		if (info.id == actionType) { found = true; break; }
	}
	if (!found) {
		m_registeredActionTypes.append({ actionType, displayName });
	}

	// Store the handler if provided
	if (handler) {
		m_actionHandlers[actionType] = std::move(handler);
	}
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
