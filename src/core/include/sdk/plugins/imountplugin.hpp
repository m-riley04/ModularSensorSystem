#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.hpp"
#include "features/mounts/mount.hpp"
#include "core_export.hpp"

class MSS_CORE_API IMountPlugin : public IPlugin
{
public:
    virtual const std::vector<ElementInfo> discover() const = 0;
    virtual Mount* createMount(const std::string& id, QObject* parent) = 0;
    virtual const IElement::Type type() const override final { return IElement::Type::Mount; }
};
