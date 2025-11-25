#pragma once

#include <QtWidgets/QWidget>
#include <QPushButton>
#include <QAction>

/**
 * @brief An extremely simple button that wraps a QAction and updates its state automatically.
 * The button does not own the action.
 * This kinda exists in toolbars, but there's not much control.
 * Seriously, why doesn't this already exist???
 */
class QActionButton : public QPushButton
{
	Q_OBJECT

public:
	QActionButton(QWidget *parent = nullptr);
	~QActionButton();

	void setShowIcon(bool showIcon);
	void setShowText(bool showText);

public slots:
	void setAction(QAction* action);

private slots:
	void initUi();
	void updateUi();

private:
	bool m_showIcon = true;
	bool m_showText = true;

	QAction* m_action = nullptr;
};

