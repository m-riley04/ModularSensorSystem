#pragma once

#include <QFrame>
#include <QStackedWidget>
#include <QPushButton>
#include "ui_qchapterswidget.h"

class QChaptersWidget : public QFrame
{
	Q_OBJECT
	Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
	Q_PROPERTY(int count READ count)
	Q_PROPERTY(bool chapterEnabled READ isChapterEnabled)
	Q_PROPERTY(bool chapterVisible READ isChapterVisible)
	Q_PROPERTY(QString chapterTitle READ chapterTitle WRITE setChapterTitle NOTIFY chapterTitleChanged)

public:
	QChaptersWidget(QWidget *parent = nullptr);
	~QChaptersWidget();

	// General methods
	int currentIndex() const { return ui.stack->currentIndex(); }
	int count() const { return ui.stack->count(); }
	
	QWidget* widget(int index) const { return ui.stack->widget(index); }

	// Chapter getters
	QString chapterTitle() const;
	bool isChapterEnabled() const;
	bool isChapterVisible() const;

public slots:
	void setCurrentIndex(int index);

	// Chapter management
	void insertWidget(int index, QWidget* widget);
	void addWidget(QWidget* widget);
	void removeWidget(QWidget* widget);

	// Chapter setters
	void setChapterTitle(const QString& title);
	void setChapterEnabled(int index, bool enabled);
	void setChapterVisible(int index, bool visible);
	void setChapterIcon(int index, const QIcon& icon);

signals:
	void currentIndexChanged(int index);
	void chapterTitleChanged(int index, const QString& title);

private:
	Ui::QChaptersWidgetClass ui;

	QPushButton* getChapterButton(int index) const;
};

