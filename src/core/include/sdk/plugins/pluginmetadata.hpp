#pragma once

/**
 * @brief Represents discovered plugin metadata (no instantiation required).
 */
struct PluginMetadata {
    std::string path; // Full library path
    uint32_t apiVersion = 0;
    uint32_t version = 0;
    std::string name; // Stable identifier (unique per plugin type)
    std::string author;
    std::string description;
    IElement::Type type = IElement::Type::Source;
    bool isCore = false;
    bool hasFactory = false; // Whether factory symbols exist
};