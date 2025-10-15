#pragma once

#include <QObject>

/**
 * @brief Basically a base class for all backend controllers in the application.
 * The main reason this was created was to provide a common parent for all backend controllers so I can
 * iterate over them.
 */
class BackendControllerBase : public QObject
{
	Q_OBJECT

public:
	BackendControllerBase(QString controllerName, QObject* parent) : QObject(parent), mName(controllerName) {}
	~BackendControllerBase() = default;

	/**
	 * @brief Returns the name of the specific controller. Initialized in each derived class.
	 * @return The name as a QString.
	 */
	QString name() const { return mName; }

protected:
	QString mName = "BackendControllerBase";
};

