#include "qchapterswidget.h"

QChaptersWidget::QChaptersWidget(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
}

QChaptersWidget::~QChaptersWidget()
{}

void QChaptersWidget::insertPage(int index, QWidget* widget) {
	if (index < 0 || index > ui.stack->count()) return;
	if (!widget) return;

	ui.stack->insertWidget(index, widget);

	// Insert corresponding button
	QPushButton* button = new QPushButton();
	button->setMinimumSize(QSize(0, 32)); // Minimum height for better appearance)
	int buttonNumber = ui.layoutButtons->count() + 1;
	button->setObjectName(
		QStringLiteral("__qt__passive_chapterButton_%1").arg(buttonNumber) // Make it a passive button for Qt Designer
	);

	const QString title =
		widget->windowTitle().isEmpty()
		? tr("Chapter %1").arg(buttonNumber)
		: widget->windowTitle();
	button->setText(title);
	widget->setWindowTitle(title); // Sync back to the widget
	button->setCheckable(true);
	ui.layoutButtons->insertWidget(index, button);

	// Connect button click to page change
	connect(button, &QPushButton::clicked, this, [this, widget]() {
		int index = ui.stack->indexOf(widget);
		setCurrentIndex(index);
		});

	// Connect widget title change to button text change. Basically initializes on load, because the window title is changed by uic.
	connect(widget, &QWidget::windowTitleChanged,
		this, [this, widget, button](const QString& newTitle) {
			int i = ui.stack->indexOf(widget);
			if (i < 0) return;

			QString title = newTitle;
			if (title.isEmpty())
				title = tr("Chapter %1").arg(i + 1);

			button->setText(title);

			if (i == currentIndex())
				ui.labelTitle->setText(title);
		});
}

void QChaptersWidget::addPage(QWidget* widget) {
	if (!widget) return;
	this->insertPage(ui.stack->count(), widget);
}

void QChaptersWidget::removePage(QWidget* widget)
{
	if (!widget) return;

	int index = ui.stack->indexOf(widget);
	if (index == -1) return;

	auto button = getPageButton(index);

	ui.stack->removeWidget(ui.stack->widget(index));
	ui.layoutButtons->removeWidget(button);

	button->deleteLater();
	widget->deleteLater();
}

void QChaptersWidget::setPageTitle(const QString& title) {
	int index = currentIndex(); if (index < 0 || index >= count()) return;

	auto button = getPageButton(index); if (!button) return;
	button->setText(title);

	auto page = widget(index); if (!page) return;
	page->setWindowTitle(title);

	emit pageTitleChanged(index, title);
}

void QChaptersWidget::setPageEnabled(int index, bool enabled) {
	if (index < 0 || index >= count()) return;
	auto button = getPageButton(index);
	if (!button) return;
	button->setEnabled(enabled);
}

void QChaptersWidget::setPageVisible(int index, bool visible) {
	if (index < 0 || index >= count()) return;
	auto button = getPageButton(index);
	if (!button) return;
	button->setVisible(visible);
}

void QChaptersWidget::setPageIcon(int index, const QIcon& icon) {
	if (index < 0 || index >= count()) return;
	auto button = getPageButton(index);
	if (!button) return;
	button->setIcon(icon);
}

QString QChaptersWidget::pageTitle() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return QString();
	auto button = getPageButton(index);
	if (!button) return QString();
	return button->text();
}

bool QChaptersWidget::isPageEnabled() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return false;
	auto button = getPageButton(index);
	if (!button) return false;
	return button->isEnabled();
}

bool QChaptersWidget::isPageVisible() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return false;
	auto button = getPageButton(index);
	if (!button) return false;
	return button->isVisible();
}

void QChaptersWidget::setCurrentIndex(int index) {
	if (index < 0 || index >= count()) return; // TODO: handle invalid index better?
	if (index == currentIndex()) return;

	ui.stack->setCurrentWidget(ui.stack->widget(index));
	ui.labelTitle->setText(pageTitle());
	updateButtonStates(index);
	emit currentIndexChanged(index);
}

void QChaptersWidget::setPageTitleVisible(bool visible)
{
	if (m_pageTitleVisible == visible) return;
	m_pageTitleVisible = visible;
	ui.labelTitle->setVisible(visible); // TODO/CONSIDER: maybe hide label per page instead?
	emit pageTitleVisibleChanged(currentIndex(), visible);
}

QPushButton* QChaptersWidget::getPageButton(int index) const {
	if (index < 0 || index >= count()) return nullptr;
	return qobject_cast<QPushButton*>(ui.layoutButtons->itemAt(index)->widget());
}

void QChaptersWidget::updateButtonStates(int checkedIndex)
{
	// Undo checks on all buttons except the checked one
	for (int i = 0; i < ui.layoutButtons->count(); ++i) {
		auto button = qobject_cast<QPushButton*>(ui.layoutButtons->itemAt(i)->widget());
		if (!button) continue;
		button->setChecked(i == checkedIndex);
	}
}