#pragma once

#include <QWidget>
#include <QStringList>

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

	struct Option {
		QString label;
		QVariant userData;
		bool operator==(const Option& other) const {
			return label == other.label && userData == other.userData;
		}
	};
private:
	const Option* findOptionByValue(const QVariant& value) const;
	const Option* findOptionByLabel(const QString& label) const;

	void rebuildMenu();
	void onActionToggled(bool);
	void openSelectionDialog();

private:
	void updateButtonText();

	QToolButton* m_button;
	QMenu* m_menu;
	QList<Option> m_options;
	QList<Option> m_selected;
	QString m_placeholderText;
	SelectionMode m_selectionMode = SelectionMode::Multi;

public:
	explicit GroupSelectWidget(QWidget* parent = nullptr);
	~GroupSelectWidget() override;

	void clear();
	void addItem(const QString& text);
	void addItem(const QString& text, const QVariant& userData);
	bool removeItem(const QString& text);
	bool hasItem(const QString& text) const;
	bool hasItem(const QVariant& userData) const;
	int count() const;

	const QList<Option>& options() const { return m_options; }
	void setOptions(const QList<Option>& options);

	const QList<Option>& selectedValues() const { return m_selected; }
	void setSelectedValues(const QList<Option>& values);
	void setSelectedUserData(const QVariantList& userData);
	QVariantList selectedUserData() const;

	QString placeholderText() const;
	void setPlaceholderText(const QString& text);

	SelectionMode selectionMode() const;
	void setSelectionMode(SelectionMode mode);

signals:
	void selectionChanged(const QList<Option>& selectedValues);
	void selectionModeChanged(GroupSelectWidget::SelectionMode newMode);

};
