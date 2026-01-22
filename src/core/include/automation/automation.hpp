#pragma once

#include <QObject>
#include <models/rule_models.hpp>

/**
 * @brief Represents an automation rule in the system.
 */
class Rule {
public:
	explicit Rule();
	explicit Rule(const RuleModel& model);

};

Q_DECLARE_METATYPE(Rule)