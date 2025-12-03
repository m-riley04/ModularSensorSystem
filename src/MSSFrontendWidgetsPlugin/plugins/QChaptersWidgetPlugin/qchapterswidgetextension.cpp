#include "qchapterswidgetextension.h"

QChaptersWidgetExtension::QChaptersWidgetExtension(QChaptersWidget* widget, QObject* parent)
    : QObject(parent), m_chaptersWidget(widget)
{}

void QChaptersWidgetExtension::addWidget(QWidget* page)
{
	if (!m_chaptersWidget) return;
	m_chaptersWidget->addWidget(page);
}

bool QChaptersWidgetExtension::canAddWidget() const
{
    return true;
}

bool QChaptersWidgetExtension::canRemove(int index) const
{
    Q_UNUSED(index);
	return m_chaptersWidget && index >= 0 && index < m_chaptersWidget->count();
}

int QChaptersWidgetExtension::count() const
{
	return m_chaptersWidget ? m_chaptersWidget->count() : 0;
}

int QChaptersWidgetExtension::currentIndex() const
{
    
    return m_chaptersWidget ? m_chaptersWidget->currentIndex() : -1;
}

void QChaptersWidgetExtension::insertWidget(int index, QWidget* widget)
{
    if (!m_chaptersWidget) return;
    m_chaptersWidget->insertWidget(index, widget);
}

void QChaptersWidgetExtension::remove(int index)
{
    if (!m_chaptersWidget) return;
    auto page = m_chaptersWidget->widget(index);
    if (page) m_chaptersWidget->removeWidget(page);
}

void QChaptersWidgetExtension::setCurrentIndex(int index)
{
    if (m_chaptersWidget) m_chaptersWidget->setCurrentIndex(index);
}

QWidget* QChaptersWidgetExtension::widget(int index) const
{
    return m_chaptersWidget ? m_chaptersWidget->widget(index) : nullptr;
}