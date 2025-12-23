#pragma once


#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QCoreApplication>
#include <controllers/loggingcontroller.hpp>

inline GstElement* createDefaultObjectDetectorProcessorFilter(const char* binName = nullptr) {
	// Initialize elements
	GstElement* bin = gst_bin_new(binName);
	GstElement* startQueue = gst_element_factory_make("queue", "inputQueue");
	GstElement* videoConvert = gst_element_factory_make("videoconvert", "videoConvert");
	GstElement* onnxinference = gst_element_factory_make("onnxinference", "inference");
	GstElement* tensorDecoder = gst_element_factory_make("ssdobjectdetector", "tensorDecoder");
	GstElement* overlay = gst_element_factory_make("objectdetectionoverlay", "overlay");
	// TODO/CONSIDER: add a video converter here if needed?
	GstElement* endQueue = gst_element_factory_make("queue", "outputQueue");

	// Check validity of each
	if (!bin || !startQueue || !videoConvert || !onnxinference || !tensorDecoder || !overlay || !endQueue) {
		LoggingController::warning("Failed to create one or more elements");
		if (bin) gst_object_unref(bin);
		if (startQueue) gst_object_unref(startQueue);
		if (videoConvert) gst_object_unref(videoConvert);
		if (onnxinference) gst_object_unref(onnxinference);
		if (tensorDecoder) gst_object_unref(tensorDecoder);
		if (overlay) gst_object_unref(overlay);
		if (endQueue) gst_object_unref(endQueue);
		return nullptr;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(bin), startQueue, videoConvert, onnxinference, tensorDecoder, overlay, endQueue, nullptr);

	/// CONFIGURATION
	// onnx
	QString appPath = QCoreApplication::applicationDirPath();
	QString yoloModel = "ssd_mobilenet_v1_coco"; // TODO: make configurable
	QString yoloPath = "C:/Users/vex10/Desktop/Local_Repos/ModularSensorSystem/src/core/yolo_models/" + yoloModel + ".onnx"; //QDir::currentPath() + "/../core/yolo_models/" + yoloModel + ".onnx";
	if (!QFile::exists(yoloPath)) {
		LoggingController::warning("ONNX model file does not exist at path: " + yoloPath);
		gst_object_unref(bin);
		return nullptr; // TODO/CONSIDER: handle better?
	}
	g_object_set(onnxinference, "model-file", yoloPath.toStdString().c_str(), nullptr); // CPU execution provider
	g_object_set(onnxinference, "execution-provider", 0, nullptr); // CPU execution provider. TODO: make configurable

	// yolo tensor decoder
	QString yoloClassesFile = "COCO_classes.txt"; // TODO: make configurable
	QString yoloClassesPath = "C:/Users/vex10/Desktop/Local_Repos/ModularSensorSystem/src/core/yolo_models/" + yoloClassesFile;
	if (!QFile::exists(yoloClassesPath)) {
		LoggingController::warning("Label file does not exist: " + yoloPath);
		gst_object_unref(bin);
		return nullptr; // TODO/CONSIDER: handle better?
	}
	g_object_set(tensorDecoder, "label-file", yoloClassesPath.toStdString().c_str(), nullptr); // TODO: make configurable
	//g_object_set(tensorDecoder, "max-detections", 100, nullptr); // TODO: make configurable

	// Link source bin to elements
	if (!gst_element_link_many(startQueue, videoConvert, onnxinference, tensorDecoder, overlay, endQueue, nullptr)) {
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