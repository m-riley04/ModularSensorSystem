#pragma once

#include <QWidget>
#include <QtWidgets>
#include "features/sources/Source/source.h"

class Source;

class SourcePropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	SourcePropertiesWidget(Source* source, QWidget* parent = nullptr);
	~SourcePropertiesWidget();

	Source* source() const { return pSource; }

	void addPage(const QString& title, QWidget* page);

private:
	Source* pSource = nullptr;
	QFrame* pFrameButtons = nullptr;
	QStackedWidget* pStackPages = nullptr;
	QVBoxLayout* pButtonLayout = nullptr;
	
};
