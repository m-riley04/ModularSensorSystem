#pragma once

/**
 * @brief The structure containing information about a detected object.
 */
struct DetectionInfo {
	const char* label;
	size_t labelLength;
	float confidence{};
	int x{}, y{}, width{}, height{};
};