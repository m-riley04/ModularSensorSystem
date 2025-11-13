#pragma once

#include <gst/gst.h>
#include <QDebug>
#include <gst/video/videooverlay.h>

/**
 * Gets the name of the video sink factory to use based on the operating system.
 */
inline const char* getVideoSinkFactoryName() {
	// TODO: Make this better and more flexible?
	const char* sinkName;
#ifdef Q_OS_WINDOWS
	sinkName = "d3dvideosink";
#elif Q_OS_LINUX
	sinkName = "v4l2sink";
#else
	sinkName = "autovideosink"; // need to change this probs
#endif
	return sinkName;
}

/**
 * @brief Allocates a default audio visualizer sink bin. Has an input pad.
 * @param windowId 
 * @param binName 
 * @return A pointer to the created audio visualizer sink bin, or nullptr on failure.
 */
inline GstElement* createDefaultAudioVisualizerSink(guintptr windowId, const char* binName = nullptr) {
	/** Multiple differet visualizers could be used here.
	 * - monoscope (best one by far)
	 * - goom (cool visuals, but not really good for real visualization)
	 * - audiovisualizers (these are a set of visualizers, but are not very good and pretty buggy on some systems)
	 *     - spacescope
	 *     - specrtascope
	 *     - synaescope
	 *     - wavescope
	 * - libvisual(?)
	 *
	 * TODO: implement a way to choose between them. */
	const char* visualizerFactoryName = "monoscope";
	const char* sinkName = getVideoSinkFactoryName();

	// Initialize elements
	GstElement* bin = gst_bin_new(binName);
	GstElement* wavescope = gst_element_factory_make(visualizerFactoryName, nullptr);
	GstElement* conv = gst_element_factory_make("videoconvert", nullptr);
	GstElement* queue = gst_element_factory_make("queue", nullptr);
	GstElement* sink = gst_element_factory_make(sinkName, nullptr);

	// Check validity of each
	if (!bin || !wavescope || !conv || !queue || !sink) {
		qWarning() << "Failed to create one or more elements";
		if (bin) gst_object_unref(bin);
		if (wavescope) gst_object_unref(wavescope);
		if (conv) gst_object_unref(conv);
		if (queue) gst_object_unref(queue);
		if (sink) gst_object_unref(sink);
		return nullptr;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(bin), wavescope, conv, queue, sink, nullptr);

	// Link source bin to elements
	if (!gst_element_link_many(wavescope, conv, queue, sink, nullptr)) {
		qWarning() << "Failed to link source bin to elements.";
		gst_object_unref(bin);
		return nullptr;
	}

	// Add input ghost pad
	GstPad* inputPad = gst_element_get_static_pad(wavescope, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("sink", inputPad);
	gst_object_unref(inputPad);
	gst_element_add_pad(bin, ghostPad);

	// Set the video sink for overlay
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(sink)) {
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
	}

	return bin;
}

inline GstElement* createDefaultDataVisualizerSink(guintptr windowId, const char* binName = nullptr) {

	const char* sinkName = getVideoSinkFactoryName();

	GstElement* bin = gst_bin_new(binName);
	GstElement* visualizer = gst_element_factory_make("analogvisualizer", "visual");
	GstElement* conv = gst_element_factory_make("videoconvert", "conv");
	GstElement* queue = gst_element_factory_make("queue", "queue");
	GstElement* video_sink = gst_element_factory_make(sinkName, "video_sink");

	if (!bin || !queue || !visualizer || !conv || !video_sink) {
		qWarning() << "Failed to create one or more elements in data-visualizer bin";
		if (bin)         gst_object_unref(bin);
		if (visualizer)   gst_object_unref(visualizer);
		if (conv)        gst_object_unref(conv);
		if (queue)       gst_object_unref(queue);
		if (video_sink)  gst_object_unref(video_sink);
		return nullptr;
	}

	// Add elements
	gst_bin_add_many(GST_BIN(bin),
		visualizer,
		conv,
		queue,
		video_sink,
		nullptr);

	// Link the chain: visualzier -> videoconvert -> queue -> video_sink
	if (!gst_element_link_many(visualizer, conv, queue, video_sink, nullptr)) {
		qWarning() << "Failed to link elements in data-visualizer bin";
		gst_object_unref(bin);
		return nullptr;
	}

	// Expose a sink ghost pad on the bin for upstream linking
	GstPad* pad_visualizer_sink = gst_element_get_static_pad(visualizer, "sink");
	GstPad* ghost_sink = gst_ghost_pad_new("sink", pad_visualizer_sink);
	gst_object_unref(pad_visualizer_sink);
	gst_pad_set_active(ghost_sink, TRUE);
	gst_element_add_pad(bin, ghost_sink);

	// Set sink window handle if needed
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(video_sink)) {
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(video_sink),
			windowId);
	}

	return bin;
}

/**
 * @brief Creates a default video sink element.
 * @param windowId The window ID for the video sink.
 * @param binName The name of the bin to create.
 * @return A pointer to the created video sink element, or nullptr on failure.
 */
inline GstElement* createDefaultVideoSink(guintptr windowId, const char* binName = nullptr) {
	const char* sinkName = getVideoSinkFactoryName();

	// Initialize elements
	GstElement* sink = gst_element_factory_make(sinkName, binName); // TODO: make this dynamic

	// Check validity of each
	if (!sink) {
		qWarning() << "Failed to create one or more elements";
		if (sink) gst_object_unref(sink);
		return nullptr;
	}

	// Set the video sink for overlay
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(sink)) {
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
	}

	return sink;
}