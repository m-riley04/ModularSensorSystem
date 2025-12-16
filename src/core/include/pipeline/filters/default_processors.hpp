#pragma once


#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <controllers/loggingcontroller.hpp>

inline GstElement* createDefaultObjectDetectorProcessorFilter(const char* binName = nullptr) {
	// Initialize elements
	GstElement* bin = gst_bin_new(binName);
	GstElement* startQueue = gst_element_factory_make("queue", "inputQueue");
	GstElement* onnxinference = gst_element_factory_make("onnxinference", "inference");
	GstElement* detector = gst_element_factory_make("ssdobjectdetector", "detector");
	GstElement* overlay = gst_element_factory_make("objectdetectionoverlay", "overlay");
	// TODO/CONSIDER: add a video converter here if needed?
	GstElement* endQueue = gst_element_factory_make("queue", "outputQueue");

	// Check validity of each
	if (!bin || !startQueue || !onnxinference || !detector || !overlay || !endQueue) {
		LoggingController::warning("Failed to create one or more elements");
		if (bin) gst_object_unref(bin);
		if (startQueue) gst_object_unref(startQueue);
		if (onnxinference) gst_object_unref(onnxinference);
		if (detector) gst_object_unref(detector);
		if (overlay) gst_object_unref(overlay);
		if (endQueue) gst_object_unref(endQueue);
		return nullptr;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(bin), startQueue, onnxinference, detector, overlay, endQueue, nullptr);

	// Link source bin to elements
	if (!gst_element_link_many(startQueue, onnxinference, detector, overlay, endQueue, nullptr)) {
		LoggingController::warning("Failed to link source bin to elements.");
		gst_object_unref(bin);
		return nullptr;
	}

	// Add input ghost pad
	GstPad* inputPad = gst_element_get_static_pad(startQueue, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);
	gst_element_add_pad(bin, ghostPad);

	// Add output ghost pad
	GstPad* outputPad = gst_element_get_static_pad(endQueue, "src");
	GstPad* ghostPadOut = gst_ghost_pad_new("src", outputPad);
	gst_object_unref(outputPad);
	gst_element_add_pad(bin, ghostPadOut);

	return bin;
}