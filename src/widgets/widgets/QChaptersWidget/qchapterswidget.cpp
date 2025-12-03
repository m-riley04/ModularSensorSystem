#include "qchapterswidget.h"

QChaptersWidget::QChaptersWidget(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
}

QChaptersWidget::~QChaptersWidget()
{}

void QChaptersWidget::insertWidget(int index, QWidget* widget) {
	if (index < 0 || index > ui.stack->count()) return;
	ui.stack->insertWidget(index, widget);
	
	QPushButton* button = new QPushButton();
	button->setText(widget->windowTitle().isEmpty() ? tr("Chapter %1").arg(index + 1) : widget->windowTitle());
	ui.layoutButtons->insertWidget(index, button);


	connect(button, &QPushButton::clicked, this, [this, widget]() {
		int index = ui.stack->indexOf(widget);
		setCurrentIndex(index);
		});
}

void QChaptersWidget::addWidget(QWidget* widget) {
	if (!widget) return;
	this->insertWidget(ui.stack->count(), widget);
}

void QChaptersWidget::removeWidget(QWidget* widget)
{
	int index = ui.stack->indexOf(widget);
	if (index == -1) return;
	auto button = getChapterButton(index);
	ui.stack->removeWidget(widget);
	ui.layoutButtons->removeWidget(button);
	button->deleteLater();
	widget->deleteLater(); // TODO: consider if this is the desired behavior
}

void QChaptersWidget::setChapterText(int index, const QString& text) {
	if (index < 0 || index >= count()) return;
	auto button = getChapterButton(index);
	if (!button) return;
	button->setText(text);
}

void QChaptersWidget::setChapterEnabled(int index, bool enabled) {
	if (index < 0 || index >= count()) return;
	auto button = getChapterButton(index);
	if (!button) return;
	button->setEnabled(enabled);
}

void QChaptersWidget::setChapterVisible(int index, bool visible) {
	if (index < 0 || index >= count()) return;
	auto button = getChapterButton(index);
	if (!button) return;
	button->setVisible(visible);
}

void QChaptersWidget::setChapterIcon(int index, const QIcon& icon) {
	if (index < 0 || index >= count()) return;
	auto button = getChapterButton(index);
	if (!button) return;
	button->setIcon(icon);
}

QString QChaptersWidget::chapterText() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return QString();
	auto button = getChapterButton(index);
	if (!button) return QString();
	return button->text();
}

bool QChaptersWidget::isChapterEnabled() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return false;
	auto button = getChapterButton(index);
	if (!button) return false;
	return button->isEnabled();
}

bool QChaptersWidget::isChapterVisible() const {
	int index = currentIndex();
	if (index < 0 || index >= count()) return false;
	auto button = getChapterButton(index);
	if (!button) return false;
	return button->isVisible();
}

void QChaptersWidget::setCurrentIndex(int index) {
	if (index < 0 || index >= count()) return; // TODO: handle invalid index better?
	if (index == currentIndex()) return;

	ui.stack->setCurrentWidget(ui.stack->widget(index));
	emit currentIndexChanged(index);
}

QPushButton* QChaptersWidget::getChapterButton(int index) const {
	if (index < 0 || index >= count()) return nullptr;
	return qobject_cast<QPushButton*>(ui.layoutButtons->itemAt(index)->widget());
}