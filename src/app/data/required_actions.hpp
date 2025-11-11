#pragma once

#include <QAction>

/**
 * The required actions for the element tree menu(s).
 */
struct ElementTreeActions {
	QAction* addSource;
	QAction* removeSource;
	QAction* editSource;

	QAction* addMount;
	QAction* removeMount;
	QAction* editMount;

	QAction* addProcessor;
	QAction* removeProcessor;
	QAction* editProcessor;
};