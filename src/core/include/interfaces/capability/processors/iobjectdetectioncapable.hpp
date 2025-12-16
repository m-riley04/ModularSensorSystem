#pragma once

#include <string>

/**
 * @brief The structure containing information about a detected object.
 */
struct DetectionInfo {
	std::string name;
	float confidence;
	int x;
	int y;
	int width;
	int height;
};

class IObjectDetectionCapable
{
public:
	virtual ~IObjectDetectionCapable() = default;

	virtual void onObjectDetected(DetectionInfo detection) = 0;

};
