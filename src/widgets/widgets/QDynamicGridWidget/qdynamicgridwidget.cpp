#include "qdynamicgridwidget.h"

QDynamicGridWidget::QDynamicGridWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

QDynamicGridWidget::~QDynamicGridWidget()
{}

void QDynamicGridWidget::addWidgetToGrid(QWidget* widget)
{
    ui.layoutGrid->addWidget(widget);
    m_widgets.append(widget);

	// Always update layout after adding a new widget
    updateLayout();
}

void QDynamicGridWidget::removeWidgetFromGrid(QWidget* widget)
{
    ui.layoutGrid->removeWidget(widget);
	widget->setParent(nullptr); // Detach the widget from this layout
	m_widgets.removeAll(widget);

    // Always update layout after removing a widget
	updateLayout();
}

void QDynamicGridWidget::updateLayout()
{
    int count = m_widgets.size();
    int columns = qCeil(qSqrt(count)); // Calculate number of columns based on the count
    int rows = qCeil(static_cast<double>(count) / columns); // Calculate number of rows based on the count
    // Clear existing layout
    QLayoutItem* item;
    while ((item = ui.layoutGrid->takeAt(0)) != nullptr) {
        // Note: We do not delete the widget here, just remove it from the layout
        ui.layoutGrid->removeItem(item);
    }
    // Re-add widgets in a grid layout
    for (int i = 0; i < count; ++i) {
        QWidget* widget = m_widgets[i];
        if (widget) {
            ui.layoutGrid->addWidget(widget, i / columns, i % columns);
        }
    }
}
