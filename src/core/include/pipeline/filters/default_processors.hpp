#pragma once


#include <gst/gst.h>
#include <qcoreapplication.h>
#include <controllers/loggingcontroller.hpp>

enum class ObjectDetectorModelType {
	SSD,
	YOLO
};

inline GstElement* createDefaultObjectDetectorProcessorFilter(ObjectDetectorModelType modelType = ObjectDetectorModelType::SSD, const char* binName = nullptr) {
	std::string modelFilenameStr;
	std::string decoderElementStr;
	std::string labelFilenameStr;
	switch (modelType) {
	case ObjectDetectorModelType::SSD:
		decoderElementStr = "ssdobjectdetector";
		modelFilenameStr = "ssd_mobilenet_v1_coco";
		labelFilenameStr = "COCO_classes.txt";
		break;
	case ObjectDetectorModelType::YOLO:
		decoderElementStr = "yolov8tensordec";
		modelFilenameStr = "yolov8s";
		labelFilenameStr = "COCO_classes.txt";
		break;
	default:
		LoggingController::warning("Unsupported ObjectDetectorModelType");
		return nullptr;
	}

	// Initialize elements
	GstElement* bin = gst_bin_new(binName);
	GstElement* videoConvert = gst_element_factory_make("videoconvert", "videoConvert");
	GstElement* videoScale = gst_element_factory_make("videoscale", "videoScale");
	GstElement* onnxinference = gst_element_factory_make("onnxinference", "inference");
	GstElement* tensorDecoder = gst_element_factory_make(decoderElementStr.c_str(), "tensorDecoder");
	GstElement* overlay = gst_element_factory_make("objectdetectionoverlay", "overlay");
	// TODO/CONSIDER: add a video converter here if needed?

	// Check validity of each
	if (!bin || !videoConvert || !videoScale || !onnxinference || !tensorDecoder || !overlay) {
		LoggingController::warning("Failed to create one or more elements");
		if (bin) gst_object_unref(bin);
		if (videoConvert) gst_object_unref(videoConvert);
		if (videoScale) gst_object_unref(videoScale);
		if (onnxinference) gst_object_unref(onnxinference);
		if (tensorDecoder) gst_object_unref(tensorDecoder);
		if (overlay) gst_object_unref(overlay);
		return nullptr;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(bin), videoConvert, videoScale, onnxinference, tensorDecoder, overlay, nullptr);

	/// CONFIGURATION

	// onnx
	QDir current = QDir::current();
	current.cdUp();
	QByteArray yoloFolderPath = current.absolutePath().toUtf8() + "/core/yolo_models/";
	QByteArray yoloPath = yoloFolderPath + QByteArray(modelFilenameStr) + ".onnx"; //QDir::currentPath() + "/../core/yolo_models/" + yoloModel + ".onnx";
	if (!QFile::exists(yoloPath)) {
		LoggingController::warning("ONNX model file does not exist at path: " + yoloPath);
		gst_object_unref(bin);
		return nullptr; // TODO/CONSIDER: handle better?
	}
	gchar* model_file = yoloPath.data();
	g_object_set(onnxinference, "model-file", model_file, nullptr);
	g_object_set(onnxinference, "execution-provider", 0, nullptr); // CPU execution provider. TODO: make configurable
	g_object_set(onnxinference, "optimization-level", 0, nullptr);

	// yolo tensor decoder
	QByteArray yoloClassesPath = yoloFolderPath + QByteArray(labelFilenameStr);
	if (!QFile::exists(yoloClassesPath)) {
		LoggingController::warning("Label file does not exist: " + yoloClassesPath);
		gst_object_unref(bin);
		return nullptr; // TODO/CONSIDER: handle better?
	}
	g_object_set(tensorDecoder, "label-file", yoloClassesPath.constData(), nullptr); // TODO: make configurable
	//g_object_set(tensorDecoder, "max-detections", 100, nullptr); // TODO: make configurable

	// Link source bin to elements
	if (!gst_element_link_many(videoScale, videoConvert, onnxinference, tensorDecoder, overlay, nullptr)) {
		LoggingController::warning("Failed to link source bin to elements.");
		gst_object_unref(bin);
		return nullptr;
	}

	// Add input ghost pad
	GstPad* inputPad = gst_element_get_static_pad(videoScale, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);
	gst_element_add_pad(bin, ghostPad);

	// Add output ghost pad
	GstPad* outputPad = gst_element_get_static_pad(overlay, "src");
	GstPad* ghostPadOut = gst_ghost_pad_new("src", outputPad);
	gst_object_unref(outputPad);
	gst_element_add_pad(bin, ghostPadOut);

	return bin;
}