#pragma once

#include <chrono>

/**
 * Nanoseconds as an unsigned long long.
 */
typedef unsigned long long ns;

/**
 * @brief Generates a timestamp in nanoseconds since epoch.
 * @return
 */
ns generateTimestampNs();