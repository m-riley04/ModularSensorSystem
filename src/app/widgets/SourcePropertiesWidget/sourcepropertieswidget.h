#pragma once

#include <QWidget>
#include <QtWidgets>
#include "features/sources/source.hpp"

class Source;

class SourcePropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	SourcePropertiesWidget(Source* source, QWidget* parent = nullptr);
	~SourcePropertiesWidget();

	Source* source() const { return m_source; }

	void addPage(const QString& title, QWidget* page);

private:
	Source* m_source = nullptr;
	QFrame* pFrameButtons = nullptr;
	QStackedWidget* pStackPages = nullptr;
	QVBoxLayout* pButtonLayout = nullptr;
	
};
