#include "automationdialog.h"
#include <widgets/AutomationRuleListItemWidget/automationrulelistitemwidget.h>

void AutomationDialog::populateRuleList()
{
	ui.listWidget->clear(); 

	auto rules = m_rulesController.rules();
	for (const auto& rule : rules) {
		auto item = new AutomationRuleItemWidget(rule);
		ui.listWidget->setItemWidget(item);
	}
}

AutomationDialog::AutomationDialog(RulesController& rc, QWidget *parent)
	: QDialog(parent), m_rulesController(rc)
{
	ui.setupUi(this);


}

AutomationDialog::~AutomationDialog()
{}

