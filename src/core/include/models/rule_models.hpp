#pragma once

#include <string>

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
struct RuleTrigger {
	int ruleId;
	std::string triggerType;
	std::string condition;
};

/**
 * @brief Represents an action associated with a rule.
 */
struct RuleAction {
	int ruleId;
	std::string actionType;
	std::string target;
};

/**
 * @brief Represents a rule with an identifier, description, and active status.
 */
struct RuleModel {
	int id;
	std::string description;
	bool isActive;
	RuleTrigger trigger;
	RuleAction action;
};
