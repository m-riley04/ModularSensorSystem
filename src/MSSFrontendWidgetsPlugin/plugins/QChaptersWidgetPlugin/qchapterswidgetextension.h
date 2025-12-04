#pragma once

#include <QObject>
#include <QDesignerContainerExtension>
#include <qchapterswidget.h>

class QChaptersWidgetExtension  : public QObject
	, public QDesignerContainerExtension
{
	Q_OBJECT
    Q_INTERFACES(QDesignerContainerExtension)

public:
	QChaptersWidgetExtension(QChaptersWidget* widget, QObject *parent);

	void addWidget(QWidget* page) override;
	bool canAddWidget() const override;
	bool canRemove(int index) const override;
	int count() const override;
	int currentIndex() const override;
	void insertWidget(int index, QWidget* widget) override;
	void remove(int index) override;
	void setCurrentIndex(int index) override;
	QWidget* widget(int index) const override;

private:
	QChaptersWidget* m_chaptersWidget;

};

