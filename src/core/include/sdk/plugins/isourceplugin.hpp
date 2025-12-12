#pragma once

#include <QObject>
#include "sdk/plugins/iplugin.hpp"
#include "features/sources/source.hpp"
#include "core_export.hpp"

// Note: SourceInfo doesn't need MSS_CORE_API - it's a simple POD struct
// defined entirely in the header with no out-of-line member functions.
struct SourceInfo {
	ElementInfo elementInfo;
	Source::Type type;
};

class MSS_CORE_API ISourcePlugin : public IPlugin
{

public:
	/// Enumerate hardware this plugin can drive
	virtual const std::vector<SourceInfo> discover() const = 0;
	virtual Source::Type sourceType() const = 0;
	virtual Source* createSource(const std::string& id, QObject* parent) = 0;
	virtual const IElement::Type type() const override final { return IElement::Type::Source; }
};
