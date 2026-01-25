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

#include <algorithm>

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
	emit selectionModeChanged(m_selectionMode);

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
	for (auto it = m_selected.begin(); it != m_selected.end();) {
		if (it->label == text || it->userData == text) it = m_selected.erase(it);
		else ++it;
	}
	if (m_options.size() == before) return false;
	rebuildMenu();
	return true;
}

bool GroupSelectWidget::hasItem(const QString& text) const
{
	return findOptionByValue(text) || findOptionByLabel(text);
}

bool GroupSelectWidget::hasItem(const QVariant& userData) const
{
	return findOptionByValue(userData) != nullptr;
}

int GroupSelectWidget::count() const
{
	return static_cast<int>(m_options.size());
}

void GroupSelectWidget::setOptions(const QList<Option>& options)
{
	if (m_options == options) return;

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
		const auto* opt = findOptionByValue(v.userData);
		if (!opt) continue;
		if (!filtered.contains(*opt)) filtered.append(*opt);
	}
	if (m_selected == filtered) return;
	m_selected = filtered;

	for (auto* a : m_menu->actions()) {
		if (!a) continue;
		const QVariant v = a->data();
		const bool checked = std::any_of(m_selected.begin(), m_selected.end(), [&v](const Option& o) { return o.userData == v; });
		QSignalBlocker blocker(a);
		a->setChecked(checked);
	}

	updateButtonText();
	emit selectionChanged(m_selected);
}

QVariantList GroupSelectWidget::selectedUserData() const
{
	QVariantList out;
	out.reserve(m_selected.size());
	for (const auto& o : m_selected) out.append(o.userData);
	return out;
}

void GroupSelectWidget::setSelectedUserData(const QVariantList& userData)
{
	QList<Option> opts;
	opts.reserve(userData.size());
	for (const auto& v : userData) {
		const auto* opt = findOptionByValue(v);
		if (!opt) continue;
		opts.append(*opt);
	}
	setSelectedValues(opts);
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
		const bool checked = std::any_of(m_selected.begin(), m_selected.end(), [&opt](const Option& o) { return o.userData == opt.userData; });
		act->setChecked(checked);
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
		const bool checked = std::any_of(m_selected.begin(), m_selected.end(), [&opt](const Option& o) { return o.userData == opt.userData; });
		item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
		item->setData(Qt::UserRole, opt.userData);
		list->addItem(item);
	}

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
	auto* clearBtn = buttons->addButton("Clear", QDialogButtonBox::ResetRole);
	QPushButton* selectAllBtn = nullptr;
	if (m_selectionMode == SelectionMode::Multi) {
		selectAllBtn = buttons->addButton("Select all", QDialogButtonBox::ActionRole);
	}
	layout->addWidget(buttons);

	QObject::connect(clearBtn, &QPushButton::clicked, &dlg, [list]() {
		for (int i = 0; i < list->count(); ++i) {
			list->item(i)->setCheckState(Qt::Unchecked);
		}
	});
	if (selectAllBtn) {
		QObject::connect(selectAllBtn, &QPushButton::clicked, &dlg, [list]() {
			for (int i = 0; i < list->count(); ++i) {
				list->item(i)->setCheckState(Qt::Checked);
			}
		});
	}

	QObject::connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	QObject::connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() != QDialog::Accepted) return;

	QList<Option> selected;
	for (int i = 0; i < list->count(); ++i) {
		auto* item = list->item(i);
		if (!item) continue;
		if (item->checkState() == Qt::Checked) {
			const QVariant ud = item->data(Qt::UserRole);
			if (const auto* opt = findOptionByValue(ud)) {
				selected.append(*opt);
			}
		}
	}

	if (m_selectionMode == SelectionMode::Single && selected.size() > 1) selected = { selected.first() };
	setSelectedValues(selected);
}

void GroupSelectWidget::onActionToggled(bool)
{
	auto* act = qobject_cast<QAction*>(sender());
	if (!act) return;
	const QVariant value = act->data();
	if (!m_menu || m_menu->signalsBlocked()) return;

	const auto* opt = findOptionByValue(value);
	if (!opt) return;

	if (m_selectionMode == SelectionMode::Single) {
		if (act->isChecked()) {
			m_selected = { *opt };
			for (auto* a : m_menu->actions()) {
				if (!a || a == act) continue;
				QSignalBlocker b(a);
				a->setChecked(false);
			}
			m_menu->hide();
		}
		else {
			m_selected.clear();
		}
	}
	else {
		if (act->isChecked()) {
			if (!m_selected.contains(*opt)) m_selected.append(*opt);
		}
		else {
			for (auto it = m_selected.begin(); it != m_selected.end();) {
				if (it->userData == value) it = m_selected.erase(it);
				else ++it;
			}
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
	for (const auto& o : m_selected) {
		labels.append(o.label);
	}
	m_button->setText(labels.join(", "));
}
