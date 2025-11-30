#pragma once

#include "sdk/plugins/iplugin.hpp"

/**
 * @brief An ABI for plugin factories.
 * mss_make_t: creates and returns a new IPlugin instance.
 * mss_destroy_t: destroys a given IPlugin instance.
 * mss_av_t: returns the API version supported by the plugin.
 */
extern "C" {
	using mss_make_t = IPlugin * ();
	using mss_destroy_t = void(IPlugin*);
	using mss_av_t = uint32_t(); // returns apiVersion()
}