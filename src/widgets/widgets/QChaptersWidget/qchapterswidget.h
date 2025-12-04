#pragma once

#include <QFrame>
#include <QStackedWidget>
#include <QPushButton>
#include "ui_qchapterswidget.h"
#include <QLabel>

class QChaptersWidget : public QFrame
{
	Q_OBJECT
	Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
	Q_PROPERTY(int count READ count)
	Q_PROPERTY(bool pageEnabled READ isPageEnabled)
	Q_PROPERTY(bool pageVisible READ isPageVisible)
	Q_PROPERTY(bool pageTitleVisible READ isPageTitleVisible WRITE setPageTitleVisible NOTIFY pageTitleVisibleChanged)
	Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle NOTIFY pageTitleChanged)

public:
	QChaptersWidget(QWidget *parent = nullptr);
	~QChaptersWidget();

	// General methods
	int currentIndex() const { return ui.stack->currentIndex(); }
	int count() const { return ui.stack->count(); }
	
	QWidget* widget(int index) const { return ui.stack->widget(index); }

	// Chapter getters
	QString pageTitle() const;
	bool isPageEnabled() const;
	bool isPageVisible() const;
	bool isPageTitleVisible() const { return m_pageTitleVisible; }

public slots:
	// Chapter management
	void insertPage(int index, QWidget* widget);
	void addPage(QWidget* widget);
	void removePage(QWidget* widget);

	// Chapter/page setters
	void setPageTitle(const QString& title);
	void setPageEnabled(int index, bool enabled);
	void setPageVisible(int index, bool visible);
	void setPageIcon(int index, const QIcon& icon);
	void setCurrentIndex(int index);
	void setPageTitleVisible(bool visible);

signals:
	void currentIndexChanged(int index);
	void pageTitleChanged(int index, const QString& title);
	void pageTitleVisibleChanged(int index, bool visible);

private:
	Ui::QChaptersWidgetClass ui;
	bool m_pageTitleVisible = true;

	QPushButton* getPageButton(int index) const;
	void updateButtonStates(int checkedIndex);
};

