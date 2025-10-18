#pragma once

#include "sdk/plugins/isourceplugin.h"

extern "C" {
	// Required exports from each plugin:
	using mss_make_t = ISourcePlugin * ();
	using mss_destroy_t = void(ISourcePlugin*);
	using mss_av_t = uint32_t(); // returns apiVersion()
}