#pragma once

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include "features/sources/source.hpp"
#include <controllers/loggingcontroller.hpp>

/**
 * @brief Creates a default video sink element.
 * @param binName The name of the bin to create.
 * @return A pointer to the created video recording sink element, or nullptr on failure.
 */
inline GstElement* createDefaultVideoRecordingSink(const char* binName = nullptr) {
	GstElement* bin = gst_bin_new(binName);

	// Elements: queue (from tee) -> valve -> x264enc -> h264parse -> mp4mux -> filesink
	GstElement* queue = gst_element_factory_make("queue", "queue");
	GstElement* valve = gst_element_factory_make("valve", "valve");
	GstElement* enc = gst_element_factory_make("x264enc", "encoder"); // TODO: consider changing encoder/parser/muxer based on source capabilities
	GstElement* parse = gst_element_factory_make("h264parse", "parser");
	GstElement* muxer = gst_element_factory_make("mp4mux", "muxer");
	GstElement* filesink = gst_element_factory_make("filesink", "filesink");

	// Validate elements
	if (!queue || !valve || !enc || !parse || !muxer || !filesink) {
		LoggingController::warning("Failed to create one or more elements for default recording sink bin");
		if (queue) { gst_object_unref(queue);     queue = nullptr; }
		if (valve) { gst_object_unref(valve);    valve = nullptr; }
		if (enc) { gst_object_unref(enc);         enc = nullptr; }
		if (parse) { gst_object_unref(parse);           parse = nullptr; }
		if (muxer) { gst_object_unref(muxer);           muxer = nullptr; }
		if (filesink) { gst_object_unref(filesink); filesink = nullptr; }
		return nullptr;
	}

	// Close valve immediately
	g_object_set(valve, "drop", true, nullptr);

	// Reasonable defaults for live capture
	g_object_set(enc,
		"tune", 0x00000004 /* zerolatency */,  // GStreamer x264enc uses flags bitfield
		nullptr);

	// Make finalized MP4 seekable earlier (optional)
	g_object_set(muxer, "faststart", TRUE, nullptr);

	// Make filesink async to avoid preroll
	g_object_set(filesink, "async", FALSE, nullptr);

	// Add to bin
	gst_bin_add_many(GST_BIN(bin), queue, valve, enc, parse, muxer, filesink, nullptr);

	// Link chain
	if (!gst_element_link_many(queue, valve, enc, parse, muxer, filesink, NULL)) {
		LoggingController::warning("Failed to link queue -> valve -> encoder -> parse -> muxer -> filesink");
		gst_object_unref(bin);
		return nullptr;
	}

	// Create bin sink ghost pad from the input queue's sink pad
	GstPad* inputPad = gst_element_get_static_pad(queue, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);

	if (!gst_element_add_pad(bin, ghostPad)) {
		LoggingController::warning("Failed to create sink ghost pad for default recording sink bin");
		gst_object_unref(bin);
		return nullptr;
	}

	return bin;
}

inline GstElement* createDefaultAudioRecordingSink(const char* binName = nullptr) {
	GstElement* bin = gst_bin_new(binName);

	// Elements: queue (from tee) -> valve -> x264enc -> h264parse -> mp4mux -> filesink
	GstElement* queue = gst_element_factory_make("queue", nullptr);
	GstElement* valve = gst_element_factory_make("valve", nullptr);
	GstElement* enc = gst_element_factory_make("wavenc", nullptr); // TODO: consider changing encoder/parser/muxer based on source capabilities
	GstElement* filesink = gst_element_factory_make("filesink", nullptr);

	// Validate elements
	if (!queue || !valve || !enc || !filesink) {
		LoggingController::warning("Failed to create one or more elements for default recording sink bin");
		if (queue) { gst_object_unref(queue);      queue = nullptr; }
		if (valve) { gst_object_unref(valve);    valve = nullptr; }
		if (enc) { gst_object_unref(enc);         enc = nullptr; }
		if (filesink) { gst_object_unref(filesink); filesink = nullptr; }
		return nullptr;
	}

	// Close valve immediately
	g_object_set(valve, "drop", true, nullptr);

	// Make filesink async to avoid preroll
	g_object_set(filesink, "async", FALSE, nullptr);

	// Add to bin
	gst_bin_add_many(GST_BIN(bin), queue, valve, enc, filesink, nullptr);

	// Link chain
	if (!gst_element_link_many(queue, valve, enc, filesink, NULL)) {
		LoggingController::warning("Failed to link queue -> valve -> encoder -> filesink");
		gst_object_unref(bin);
		return nullptr;
	}

	// Create bin sink ghost pad from the input queue's sink pad
	GstPad* inputPad = gst_element_get_static_pad(queue, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);

	if (!gst_element_add_pad(bin, ghostPad)) {
		LoggingController::warning("Failed to create sink ghost pad for default recording sink bin");
		gst_object_unref(bin);
		return nullptr;
	}

	return bin;
}

inline GstElement* createDefaultDataRecordingSink(const char* binName = nullptr) {
	GstElement* bin = gst_bin_new(binName);

	// Elements: queue (from tee) -> valve -> x264enc -> h264parse -> mp4mux -> filesink
	GstElement* queue = gst_element_factory_make("queue", nullptr);
	GstElement* valve = gst_element_factory_make("valve", nullptr);
	//GstElement* enc = gst_element_factory_make("x264enc", nullptr); // TODO: consider adding encoder/parser/muxer based on source capabilities
	GstElement* filesink = gst_element_factory_make("filesink", nullptr);

	// Validate elements
	if (!queue || !valve || !filesink) {
		LoggingController::warning("Failed to create one or more elements for default recording sink bin");
		if (queue) { gst_object_unref(queue);      queue = nullptr; }
		if (valve) { gst_object_unref(valve);    valve = nullptr; }
		if (filesink) { gst_object_unref(filesink); filesink = nullptr; }
		return nullptr;
	}

	// Close valve immediately
	g_object_set(valve, "drop", true, nullptr);

	// Make filesink async to avoid preroll
	g_object_set(filesink, "async", FALSE, nullptr);

	// Add to bin
	gst_bin_add_many(GST_BIN(bin), queue, valve, filesink, nullptr);

	// Link chain
	if (!gst_element_link_many(queue, valve, filesink, NULL)) {
		LoggingController::warning("Failed to link queue -> valve -> filesink");
		gst_object_unref(bin);
		return nullptr;
	}
	
	// Create bin sink ghost pad from the input queue's sink pad
	GstPad* inputPad = gst_element_get_static_pad(queue, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);

	if (!gst_element_add_pad(bin, ghostPad)) {
		LoggingController::warning("Failed to create sink ghost pad for default recording sink bin");
		gst_object_unref(bin);
		return nullptr;
	}

	return bin;
}

inline GstElement* createDefaultRecordingSink(Source::Type type, const char* binName = nullptr) {
	switch (type) {
	case Source::Type::VIDEO:
		return createDefaultVideoRecordingSink(binName);
	case Source::Type::AUDIO:
		return createDefaultAudioRecordingSink(binName);
	case Source::Type::DATA:
		return createDefaultDataRecordingSink(binName);
	default:
		LoggingController::warning("No default recording sink available for the given source type.");
		return nullptr;
	}
}