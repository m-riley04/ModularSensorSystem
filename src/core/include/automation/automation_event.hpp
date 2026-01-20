#pragma once

#include <QMetaType>
#include <QVariantMap>
#include <QUuid>
#include <QString>
#include <utils/session_utils.hpp>

struct AutomationEvent {
	QString type;
	QUuid elementId;
	QVariantMap payload;
	ns timestamp = 0;
};

Q_DECLARE_METATYPE(AutomationEvent)
