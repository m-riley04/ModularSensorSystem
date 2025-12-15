#pragma once

#include <string>
#include <optional>
#include "features/element.hpp"
#include "core_export.hpp"

struct MountError {
	std::string msg;
};

/**
 * @brief Mounts represent the physical "mount" that physical sources sit on. They can provide data
 */
class MSS_CORE_API Mount : public Element
{
	Q_OBJECT

public:
	Mount(const ElementInfo& element, QObject* parent) : Element(element, parent) {}
	virtual ~Mount() = default;

	virtual MountError error() const = 0;

	virtual const IElement::Type elementType() const noexcept override final { return IElement::Type::Mount; }

signals:
	void dataUpdated(QByteArray data);
	void errorOccurred(const QString& errorMessage);

};

