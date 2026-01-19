#pragma once

#include <string>
#include <models/processing_models.hpp>

class IObjectDetectionCapable
{
public:
	virtual ~IObjectDetectionCapable() = default;

	virtual void onObjectDetected(DetectionInfo detection) = 0;

};
