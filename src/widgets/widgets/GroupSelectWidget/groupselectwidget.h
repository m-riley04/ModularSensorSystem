#pragma once

#include <QWidget>
#include <QStringList>

#include <vector>

class QToolButton;
class QMenu;
class QAction;
class QDialog;

class GroupSelectWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
public:
	enum class SelectionMode {
		Single,
		Multi
	};
	Q_ENUM(SelectionMode)

	explicit GroupSelectWidget(QWidget* parent = nullptr);
	~GroupSelectWidget() override;

	void clear();
	void addItem(const QString& text);
	void addItem(const QString& text, const QString& value);
	void addItems(const QStringList& items);
	bool removeItem(const QString& text);
	bool hasItem(const QString& text) const;
	int count() const;

	QStringList options() const;
	void setOptions(const QStringList& options);

	QStringList selectedValues() const;
	void setSelectedValues(const QStringList& values);

	QString placeholderText() const;
	void setPlaceholderText(const QString& text);

	SelectionMode selectionMode() const;
	void setSelectionMode(SelectionMode mode);

signals:
	void selectionChanged(const QStringList& selectedValues);

private:
	struct Option {
		QString label;
		QString value;
	};

	const Option* findOptionByValue(const QString& value) const;
	const Option* findOptionByLabel(const QString& label) const;

	void rebuildMenu();
	void onActionToggled(bool);
	void openSelectionDialog();

private:
	void updateButtonText();

	QToolButton* m_button;
	QMenu* m_menu;
	std::vector<Option> m_options;
	QStringList m_selected;
	QString m_placeholderText;
	SelectionMode m_selectionMode = SelectionMode::Multi;
};
