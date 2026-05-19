#pragma once

#include <QObject>
#include "models/rule_models.hpp"
#include "core_export.hpp"
#include <automation/automation_event.hpp>
#include <vector>
#include <functional>
#include <unordered_map>
#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>

/// Describes a registered event type or action type for UI display.
struct AutomationTypeInfo {
	QString id;          // e.g. "processor.objectDetected"
	QString displayName; // e.g. "Object detected"
};

class MSS_CORE_API RulesController : public QObject
{
	Q_OBJECT

public:
	using ActionHandler = std::function<void(const RuleAction&)>;

	explicit RulesController(SessionController& sc, ElementsController& ec, QObject* parent);
	~RulesController();

	const std::vector<Rule>& rules() const { return m_rules; }

	void addRule(const Rule& rule) { m_rules.push_back(rule); }
	bool updateRule(int index, const Rule& rule);
	bool removeRule(int index);

	/// Register a known event type so it appears in the UI.
	void registerEventType(const QString& id, const QString& displayName);

	/// Register a named action type with an optional handler.
	/// If no handler is provided the type still appears in the UI
	/// but a warning is logged at dispatch time.
	void registerAction(const QString& actionType, const QString& displayName,
						ActionHandler handler = {});

	/// Ordered lists the UI iterates to populate dropdowns.
	const QList<AutomationTypeInfo>& registeredEventTypes() const { return m_registeredEventTypes; }
	const QList<AutomationTypeInfo>& registeredActionTypes() const { return m_registeredActionTypes; }

private:
	ElementsController& m_elementsController;
	SessionController& m_sessionController;

	std::vector<Rule> m_rules;
	std::unordered_map<QString, ActionHandler> m_actionHandlers;

	QList<AutomationTypeInfo> m_registeredEventTypes;
	QList<AutomationTypeInfo> m_registeredActionTypes;

	void onAutomationEvent(const AutomationEvent& event);
	void executeRuleAction(const RuleAction& action);

	// built-in registers
	void registerBuiltInEventTypes();
	void registerBuiltInActions();
};