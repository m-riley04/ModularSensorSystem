#pragma once

#include <string_view>

/**
 * @brief Represents a trigger for a rule.
 */
struct RuleTrigger {
	int ruleId;
	std::string_view triggerType;
	std::string_view condition;
};

/**
 * @brief Represents an action associated with a rule.
 */
struct RuleAction {
	int ruleId;
	std::string_view actionType;
	std::string_view target;
};

/**
 * @brief Represents a rule with an identifier, description, and active status.
 */
struct RuleModel {
	int id;
	std::string_view description;
	bool isActive;
	RuleTrigger trigger;
	RuleAction action;
};
