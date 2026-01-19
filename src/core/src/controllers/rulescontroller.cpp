#include <controllers/rulescontroller.hpp>

RulesController::RulesController()
{

}

RulesController::~RulesController()
{

}

void RulesController::addRule(const Rule rule)
{
	m_rules.push_back(rule);
}
