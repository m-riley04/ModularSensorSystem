#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QIcon>
#include <QString>
#include <QWidget>

class GroupSelectWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	explicit GroupSelectWidgetPlugin(QObject* parent = nullptr);

	bool isContainer() const override;
	bool isInitialized() const override;
	QIcon icon() const override;
	QString domXml() const override;
	QString group() const override;
	QString includeFile() const override;
	QString name() const override;
	QString toolTip() const override;
	QString whatsThis() const override;
	QWidget* createWidget(QWidget* parent) override;
	void initialize(QDesignerFormEditorInterface* core) override;

private:
	bool initialized = false;
};
