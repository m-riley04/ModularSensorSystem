#pragma once

#include <string>
#include <features/ielement.hpp>

inline static std::string elementTypeToString(IElement::Type type) {
    switch (type) {
    case IElement::Type::Source:
        return "Source";
    case IElement::Type::Processor:
        return "Processor";
    case IElement::Type::Mount:
        return "Mount";
    default:
        return "Unknown";
    }
}