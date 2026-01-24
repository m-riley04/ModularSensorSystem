#include "groupselectwidget.h"

#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QPushButton>

GroupSelectWidget::GroupSelectWidget(QWidget* parent)
	: QWidget(parent)
	, m_button(new QToolButton(this))
	, m_menu(new QMenu(this))
{
	m_placeholderText = "Select...";

	auto* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_button);

	m_button->setText(m_placeholderText);
	m_button->setPopupMode(QToolButton::DelayedPopup);
	m_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	m_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	connect(m_button, &QToolButton::clicked, this, [this]() { openSelectionDialog(); });

	rebuildMenu();
}

GroupSelectWidget::~GroupSelectWidget() = default;

const GroupSelectWidget::Option* GroupSelectWidget::findOptionByValue(const QVariant& userData) const
{
	for (const auto& o : m_options) {
		if (o.userData == userData) return &o;
	}
	return nullptr;
}

const GroupSelectWidget::Option* GroupSelectWidget::findOptionByLabel(const QString& label) const
{
	for (const auto& o : m_options) {
		if (o.label == label) return &o;
	}
	return nullptr;
}

GroupSelectWidget::SelectionMode GroupSelectWidget::selectionMode() const
{
	return m_selectionMode;
}

void GroupSelectWidget::setSelectionMode(SelectionMode mode)
{
	if (m_selectionMode == mode) return;
	m_selectionMode = mode;

	if (m_selectionMode == SelectionMode::Single && m_selected.size() > 1) {
		setSelectedValues({ m_selected.first() });
	}
}

void GroupSelectWidget::clear()
{
	m_options.clear();
	m_selected.clear();
	rebuildMenu();
}

void GroupSelectWidget::addItem(const QString& text)
{
	if (text.isEmpty()) return;
	if (findOptionByValue(text)) return;
	m_options.push_back({ text, text });
	rebuildMenu();
}

void GroupSelectWidget::addItem(const QString& text, const QVariant& userData)
{
	if (text.isEmpty()) return;
	const QVariant v = userData.isNull() ? text : userData;
	if (findOptionByValue(v)) return;
	m_options.push_back({ text, v });
	rebuildMenu();
}

bool GroupSelectWidget::removeItem(const QString& text)
{
	const auto before = m_options.size();
	for (auto it = m_options.begin(); it != m_options.end();) {
		if (it->label == text || it->userData == text) it = m_options.erase(it);
		else ++it;
	}
	m_selected.removeAll(text);
	if (m_options.size() == before) return false;
	rebuildMenu();
	return true;
}

bool GroupSelectWidget::hasItem(const QString& text) const
{
	return findOptionByValue(text) || findOptionByLabel(text);
}

int GroupSelectWidget::count() const
{
	return static_cast<int>(m_options.size());
}

void GroupSelectWidget::setOptions(const QList<Option>& options)
{
	QList<Option> curr;
	for (const auto& o : m_options) curr.append(o);
	if (curr == options) return;

	m_options.clear();
	for (const auto& o : options) {
		if (o.label.isEmpty()) continue;
		m_options.push_back(o);
	}

	// filter selected
	QList<Option> filtered;
	for (const auto& s : m_selected) {
		if (findOptionByValue(s.userData)) filtered.append(s);
	}
	m_selected = filtered;
	rebuildMenu();
	updateButtonText();
	emit selectionChanged(m_selected);
}

void GroupSelectWidget::setSelectedValues(const QList<Option>& values)
{
	QList<Option> filtered;
	for (const auto& v : values) {
		if (findOptionByValue(v.userData) && !filtered.contains(v)) filtered.append(v);
	}
	if (m_selected == filtered) return;
	m_selected = filtered;

	for (auto* a : m_menu->actions()) {
		if (!a) continue;
		const bool checked = m_selected.contains(a->data().toString());
		QSignalBlocker blocker(a);
		a->setChecked(checked);
	}

	updateButtonText();
	emit selectionChanged(m_selected);
}

QString GroupSelectWidget::placeholderText() const
{
	return m_placeholderText;
}

void GroupSelectWidget::setPlaceholderText(const QString& text)
{
	if (m_placeholderText == text) return;
	m_placeholderText = text;
	updateButtonText();
}

void GroupSelectWidget::rebuildMenu()
{
	QSignalBlocker menuBlocker(m_menu);
	m_menu->clear();

	for (const auto& opt : m_options) {
		auto* act = m_menu->addAction(opt.label);
		act->setCheckable(true);
		act->setData(opt.userData);
		act->setChecked(m_selected.contains(opt.userData));
		connect(act, &QAction::toggled, this, &GroupSelectWidget::onActionToggled);
	}

	updateButtonText();
}

void GroupSelectWidget::openSelectionDialog()
{
	QDialog dlg(this);
	dlg.setWindowTitle(m_placeholderText.isEmpty() ? QString("Select") : m_placeholderText);
	dlg.setModal(true);

	auto* layout = new QVBoxLayout(&dlg);

	auto* list = new QListWidget(&dlg);
	list->setSelectionMode(QAbstractItemView::NoSelection);
	layout->addWidget(list);

	for (const auto& opt : m_options) {
		auto* item = new QListWidgetItem(opt.label);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(m_selected.contains(opt.userData) ? Qt::Checked : Qt::Unchecked);
		item->setData(Qt::UserRole, opt.userData);
		list->addItem(item);
	}

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
	auto* clearBtn = buttons->addButton("Clear", QDialogButtonBox::ResetRole);
	layout->addWidget(buttons);

	QObject::connect(clearBtn, &QPushButton::clicked, &dlg, [list]() {
		for (int i = 0; i < list->count(); ++i) {
			list->item(i)->setCheckState(Qt::Unchecked);
		}
	});

	QObject::connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() != QDialog::Accepted) return;

	QStringList selected;
	for (int i = 0; i < list->count(); ++i) {
		auto* item = list->item(i);
		if (!item) continue;
		if (item->checkState() == Qt::Checked) selected.append(item->data(Qt::UserRole).toString());
	}

	if (m_selectionMode == SelectionMode::Single && selected.size() > 1) {
		selected = { selected.first() };
	}
	setSelectedValues(selected);
}

void GroupSelectWidget::onActionToggled(bool)
{
	auto* act = qobject_cast<QAction*>(sender());
	if (!act) return;
	const QString value = act->data().toString();
	if (!m_menu || m_menu->signalsBlocked()) return;

	if (m_selectionMode == SelectionMode::Single) {
		if (act->isChecked()) {
			// clear all others
			m_selected = { value };
			for (auto* a : m_menu->actions()) {
				if (a == act) continue;
				a->blockSignals(true);
				a->setChecked(false);
				a->blockSignals(false);
			}
			m_menu->hide();
		}
		else {
			m_selected.removeAll(value);
		}
	}
	else {
		if (act->isChecked()) {
			if (!m_selected.contains(value)) m_selected.append(value);
		}
		else {
			m_selected.removeAll(value);
		}
	}

	updateButtonText();
	emit selectionChanged(m_selected);
}

void GroupSelectWidget::updateButtonText()
{
	if (m_selected.isEmpty()) {
		m_button->setText(m_placeholderText);
		return;
	}
	QStringList labels;
	labels.reserve(m_selected.size());
	for (const auto& v : m_selected) {
		if (const auto* opt = findOptionByValue(v)) labels.append(opt->label);
		else labels.append(v);
	}
	m_button->setText(labels.join(", "));
}
