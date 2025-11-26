#pragma once

#include <QString>
#include <QDir>
#include <QDebug>
#include <gst/gst.h>

typedef unsigned long long ns;
struct SessionProperties;
class Source;

const QString generateSessionDirectoryPath(const SessionProperties& props, const QString suffix);

const QString generateSessionSourcePath(Source* src, const SessionProperties& props, const ns timestamp);

/**
 * @brief Sanitizes a file name to be safe for use on Windows file systems.
 * @param name The original file name.
 * @return The sanitized file name.
 */
std::string sanitizeFileNameForWindows(const std::string& name);