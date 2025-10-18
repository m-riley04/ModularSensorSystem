#pragma once

#include "sdk/plugins/iplugin.h"

extern "C" {
	// Required exports from each plugin:
	using mss_make_t = IPlugin * ();
	using mss_destroy_t = void(IPlugin*);
	using mss_av_t = uint32_t(); // returns apiVersion()
}