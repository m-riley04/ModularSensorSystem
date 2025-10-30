#include "sessionpropertiesdialog.h"

SessionPropertiesDialog::SessionPropertiesDialog(MainController* controller, QWidget *parent)
	: QDialog(parent), pController(controller)
{
	ui.setupUi(this);

	initSignals();
}

SessionPropertiesDialog::~SessionPropertiesDialog()
{}

void SessionPropertiesDialog::initSignals()
{
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &SessionPropertiesDialog::transmit);
}

void SessionPropertiesDialog::transmit(QAbstractButton* button) {

	auto role = ui.buttonBox->buttonRole(button);
	
	switch (role) {
	case (QDialogButtonBox::ApplyRole):
		this->close();
		break;
	case (QDialogButtonBox::RejectRole):
		this->close();
		break;
	case (QDialogButtonBox::ResetRole):
		// TODO: Do this
		break;
	default:
		this->close();
		break;
	}
}