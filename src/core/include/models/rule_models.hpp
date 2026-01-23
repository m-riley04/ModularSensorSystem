#pragma once

#include <string>

class RuleTrigger;
class RuleAction;
class Rule;

enum class RuleTriggerType
{
	AutomationEventType,
};

enum class RuleActionType
{
	SessionStartRecording,
	SessionStopRecording,
	SessionStartProcessing,
	SessionStopProcessing,
};

inline std::string toString(RuleTriggerType t)
{
	switch (t) {
		case RuleTriggerType::AutomationEventType: return "automation.event";
		default: return {};
	}
}

inline bool tryParseRuleTriggerType(const std::string& s, RuleTriggerType& out)
{
	if (s == "automation.event") { out = RuleTriggerType::AutomationEventType; return true; }
	return false;
}

inline std::string toString(RuleActionType t)
{
	switch (t) {
		case RuleActionType::SessionStartRecording: return "session.startRecording";
		case RuleActionType::SessionStopRecording: return "session.stopRecording";
		case RuleActionType::SessionStartProcessing: return "session.startProcessing";
		case RuleActionType::SessionStopProcessing: return "session.stopProcessing";
		default: return {};
	}
}

inline bool tryParseRuleActionType(const std::string& s, RuleActionType& out)
{
	if (s == "session.startRecording") { out = RuleActionType::SessionStartRecording; return true; }
	if (s == "session.stopRecording") { out = RuleActionType::SessionStopRecording; return true; }
	if (s == "session.startProcessing") { out = RuleActionType::SessionStartProcessing; return true; }
	if (s == "session.stopProcessing") { out = RuleActionType::SessionStopProcessing; return true; }
	return false;
}

/**
 * @brief Represents a trigger for a rule.
 */
class RuleTrigger
{
public:
	RuleTrigger() = default;
	RuleTrigger(int ruleId, RuleTriggerType triggerType, std::string condition)
		: m_ruleId(ruleId)
		, m_triggerType(triggerType)
		, m_condition(std::move(condition))
	{}

	int ruleId() const { return m_ruleId; }
	void setRuleId(int id) { m_ruleId = id; }

	RuleTriggerType triggerType() const { return m_triggerType; }
	void setTriggerType(RuleTriggerType t) { m_triggerType = t; }

	const std::string& condition() const { return m_condition; }
	void setCondition(std::string c) { m_condition = std::move(c); }

private:
	int m_ruleId = -1;
	RuleTriggerType m_triggerType = RuleTriggerType::AutomationEventType;
	std::string m_condition;
};

/**
 * @brief Represents an action associated with a rule.
 */
class RuleAction
{
public:
	RuleAction() = default;
	RuleAction(int ruleId, RuleActionType actionType, std::string target)
		: m_ruleId(ruleId)
		, m_actionType(actionType)
		, m_target(std::move(target))
	{}

	int ruleId() const { return m_ruleId; }
	void setRuleId(int id) { m_ruleId = id; }

	RuleActionType actionType() const { return m_actionType; }
	void setActionType(RuleActionType t) { m_actionType = t; }

	const std::string& target() const { return m_target; }
	void setTarget(std::string t) { m_target = std::move(t); }

private:
	int m_ruleId = -1;
	RuleActionType m_actionType = RuleActionType::SessionStartRecording;
	std::string m_target;
};

/**
 * @brief Represents a rule with an identifier, description, and active status.
 */
class Rule
{
public:
	Rule() = default;
	Rule(int id, std::string description, bool isActive, RuleTrigger trigger, RuleAction action)
		: m_id(id)
		, m_description(std::move(description))
		, m_isActive(isActive)
		, m_trigger(std::move(trigger))
		, m_action(std::move(action))
	{}

	int id() const { return m_id; }
	void setId(int id) { m_id = id; }

	const std::string& description() const { return m_description; }
	void setDescription(std::string d) { m_description = std::move(d); }

	bool isActive() const { return m_isActive; }
	void setActive(bool a) { m_isActive = a; }

	const RuleTrigger& trigger() const { return m_trigger; }
	RuleTrigger& trigger() { return m_trigger; }

	const RuleAction& action() const { return m_action; }
	RuleAction& action() { return m_action; }

private:
	int m_id = -1;
	std::string m_description;
	bool m_isActive = true;
	RuleTrigger m_trigger;
	RuleAction m_action;
};
