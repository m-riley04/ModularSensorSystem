#pragma once

#include <string_view>
#include <vector>

/**
 * @brief Represents a rule with an identifier, description, and active status.
 */
struct Rule {
	int id;
	std::string_view description;
	bool isActive;
};

/**
 * @brief A controller class for managing rules within the application.
 * Rules are for automating actions based on processor events.
 */
class RulesController {
private:
	std::vector<Rule> m_rules;

public:
	explicit RulesController();
	~RulesController();

	void addRule(const Rule rule);

	void applyRule(int ruleId);
	bool validateRule(int ruleId) const;
};