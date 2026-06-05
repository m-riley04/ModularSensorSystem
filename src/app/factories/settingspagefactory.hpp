#include <QWidget>
#include "interfaces/capability/general/isettingsprovider.hpp"

/**
 * @brief Factory class responsible for creating settings pages based on an ISettingsProvider.
 */
static class SettingsPageFactory
{
public:
	QWidget* build(ISettingsProvider* provider, QWidget* parent = nullptr);

};