#include "automation/automation.hpp"

Rule::Rule(const RuleModel& model, QObject* parent)
	: QObject(parent)
{

}


void Rule::trigger()
{


	emit triggered();
}