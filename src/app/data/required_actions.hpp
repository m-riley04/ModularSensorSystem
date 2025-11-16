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

/**
 * The required actions for the session controls.
 */
struct SessionControlsActions {
	QAction* startStopSession;
	QAction* recordSession;
	QAction* restartSession;
};