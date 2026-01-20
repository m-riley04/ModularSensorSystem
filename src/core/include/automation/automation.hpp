#pragma once

#include <QObject>
#include <models/rule_models.hpp>

class Rule : public QObject {
	Q_OBJECT
private:


public:
	explicit Rule(const RuleModel& model, QObject* parent);

public slots:
	void trigger();

signals:
	void triggered();
};