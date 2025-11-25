#include "qactionbutton.h"


QActionButton::QActionButton(QWidget *parent)
	: QPushButton(parent)
{}

QActionButton::~QActionButton()
{}

void QActionButton::setShowIcon(bool showIcon)
{
	m_showIcon = showIcon;
}

void QActionButton::setShowText(bool showText)
{
	m_showText = showText;
}

void QActionButton::initUi()
{
	setText("No Action");
	setEnabled(false);
	setCheckable(false);
}

void QActionButton::updateUi() {
	if (!m_action) {
		initUi();
		return;
	}

	if (m_showText) setText(m_action->text()); else setText(QString());
	setEnabled(m_action->isEnabled());
	setCheckable(m_action->isCheckable());
	if (m_action->isCheckable()) {
		setChecked(m_action->isChecked());
	}
	if (m_showIcon) setIcon(m_action->icon()); else setIcon(QIcon());
}

void QActionButton::setAction(QAction* action) {
	if (m_action == action) return;

	m_action = action;

	if (action == nullptr) {
		initUi();
		return;
	}
	connect(this, &QPushButton::clicked, m_action, &QAction::trigger);
	connect(m_action, &QAction::changed, this, &QActionButton::updateUi);

	updateUi();
}