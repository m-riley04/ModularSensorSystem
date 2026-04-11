#pragma once

#include <QStringLiteral>

class RuleTrigger;
class RuleAction;
class Rule;

// Built-in event types and actions that can be used in rule triggers
namespace AutomationEventStrings {
	// Pipeline events
	inline const QString PipelineStateChanged = QStringLiteral("pipeline.stateChanged");
	inline const QString PipelineEos = QStringLiteral("pipeline.eos");
	inline const QString PipelineError = QStringLiteral("pipeline.error");
	inline const QString RecordingStarted = QStringLiteral("pipeline.recordingStarted");
	inline const QString RecordingStopped = QStringLiteral("pipeline.recordingStopped");

	// Processor / detection events
	inline const QString ProcessorObjectDetected = QStringLiteral("processor.objectDetected");

	// Session lifecycle events
	inline const QString SessionStarted = QStringLiteral("session.started");
	inline const QString SessionStopped = QStringLiteral("session.stopped");

	// Processing lifecycle events
	inline const QString ProcessingStarted = QStringLiteral("processor.processingStarted");
	inline const QString ProcessingStopped = QStringLiteral("processor.processingStopped");
}

namespace AutomationActionStrings {
	inline const QString SessionStartRecording = QStringLiteral("session.startRecording");
	inline const QString SessionStopRecording = QStringLiteral("session.stopRecording");
	inline const QString SessionStartProcessing = QStringLiteral("session.startProcessing");
	inline const QString SessionStopProcessing = QStringLiteral("session.stopProcessing");
}

/**
 * @brief Represents a trigger for a rule.
 */
class RuleTrigger
{
public:
	RuleTrigger() = default;
	RuleTrigger(int ruleId, QString eventType, QString condition)
		: m_ruleId(ruleId)
		, m_eventType(std::move(eventType))
		, m_condition(std::move(condition))
	{}

	int ruleId() const { return m_ruleId; }
	void setRuleId(int id) { m_ruleId = id; }

	/// The event type string to match against AutomationEvent::type
	const QString& eventType() const { return m_eventType; }
	void setEventType(QString t) { m_eventType = std::move(t); }

	const QString& condition() const { return m_condition; }
	void setCondition(QString c) { m_condition = std::move(c); }

private:
	int m_ruleId = -1;
	QString m_eventType;  // e.g. "pipeline.eos", "processor.objectDetected", etc.
	QString m_condition;
};

/**
 * @brief Represents an action associated with a rule.
 */
class RuleAction
{
public:
	RuleAction() = default;
	RuleAction(int ruleId, QString actionType, QString target)
		: m_ruleId(ruleId)
		, m_actionType(std::move(actionType))
		, m_target(std::move(target))
	{}

	int ruleId() const { return m_ruleId; }
	void setRuleId(int id) { m_ruleId = id; }

	/// A dot-namespaced action identifier, e.g. "session.startRecording"
	const QString& actionType() const { return m_actionType; }
	void setActionType(QString t) { m_actionType = std::move(t); }

	const QString& target() const { return m_target; }
	void setTarget(QString t) { m_target = std::move(t); }

private:
	int m_ruleId = -1;
	QString m_actionType;
	QString m_target;
};

/**
 * @brief Represents a rule with an identifier, description, and active status.
 */
class Rule
{
public:
	Rule() = default;
	Rule(int id, QString description, bool isActive, RuleTrigger trigger, RuleAction action)
		: m_id(id)
		, m_description(std::move(description))
		, m_isActive(isActive)
		, m_trigger(std::move(trigger))
		, m_action(std::move(action))
	{}

	int id() const { return m_id; }
	void setId(int id) { m_id = id; }

	const QString& description() const { return m_description; }
	void setDescription(QString d) { m_description = std::move(d); }

	bool isActive() const { return m_isActive; }
	void setActive(bool a) { m_isActive = a; }

	const RuleTrigger& trigger() const { return m_trigger; }
	RuleTrigger& trigger() { return m_trigger; }

	const RuleAction& action() const { return m_action; }
	RuleAction& action() { return m_action; }

private:
	int m_id = -1;
	QString m_description;
	bool m_isActive = true;
	RuleTrigger m_trigger;
	RuleAction m_action;
};