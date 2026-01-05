#pragma once

#include <gst/gst.h>
#include <string>
#include <boost/uuid.hpp>
#include <features/element.hpp>
#include <core_export.hpp>

class BinBase {
public:
	virtual ~BinBase() {
		if (m_bin) {
			gst_object_unref(m_bin);
			m_bin = nullptr;
		}
	}

	GstElement* bin() const { return m_bin; }

protected:
	explicit BinBase(Element* element)
		: m_bin(nullptr), m_element(element)
	{
		if (!element) return;

		create(element->id().c_str());
	}

	/**
	 * @brief Helper to create a bin element with a given name.
	 * @param name 
	 * @return 
	 */
	bool create(const char* name) {
		// TODO: is this where we should set the bin to NULL state and remove from parent?
		if (m_bin) {
			gst_object_unref(m_bin);
			m_bin = nullptr;
		}

		m_bin = gst_bin_new(name);
		return m_bin != nullptr;
	}

	/**
	 * @brief A helper to add multiple elements to a bin.
	 * TODO/CONSIDER: gst_bin_add_many() exists. Is there a way to pass to this directly?
	 * @param elems 
	 */
	template <typename... GstElements>
	bool addMany(GstElement* first, GstElements... rest) {
		static_assert((std::is_same_v<GstElements, GstElement*> && ...), "addMany requires GstElement* arguments");

		// Check if bin is valid
		if (!m_bin) return false;

		gst_bin_add_many(GST_BIN(m_bin), first, rest..., NULL);

		return true;
	}

	/**
	 * Creates a ghost pad from an element's static pad.
	 */
	GstPad* makeGhostPad(const char* ghostName, GstElement* element, const char* padName) {
		GstPad* pad = gst_element_get_static_pad(element, padName);
		if (!pad) return nullptr;
		GstPad* ghost = gst_ghost_pad_new(ghostName, pad);
		gst_object_unref(pad);
		if (!ghost) return nullptr;
		gst_element_add_pad(m_bin, ghost);
		return ghost;
	}

	GstPad* makeRequestGhostPad(const char* ghostName, GstElement* element, const char* padName) {
		GstPad* pad = gst_element_request_pad_simple(element, padName);
		if (!pad) return nullptr;
		GstPad* ghost = gst_ghost_pad_new(ghostName, pad);
		gst_object_unref(pad);
		if (!ghost) return nullptr;
		gst_element_add_pad(m_bin, ghost);
		return ghost;
	}

	/**
	 * @brief Creates a "sometimes" (request) sink ghost pad on the bin using a pad template.
	 * This allows external elements to request pads from this bin dynamically.
	 * @param ghostName The name of the ghost pad (e.g., "sink_%u" for templated names)
	 * @param targetElement The internal element whose static pad will be the target
	 * @param targetPadName The name of the target element's static pad
	 * @return The created ghost pad, or nullptr on failure
	 */
	GstPad* makeSometimesSinkGhostPad(const char* ghostName, GstElement* targetElement, const char* targetPadName) {
		if (!m_bin || !targetElement) return nullptr;

		// Get the target pad from the internal element
		GstPad* targetPad = gst_element_get_static_pad(targetElement, targetPadName);
		if (!targetPad) return nullptr;

		// Create a pad template for the sometimes pad
		GstStaticPadTemplate padTemplate = GST_STATIC_PAD_TEMPLATE(
			ghostName,
			GST_PAD_SINK,
			GST_PAD_SOMETIMES,
			GST_STATIC_CAPS_ANY
		);
		GstPadTemplate* templ = gst_static_pad_template_get(&padTemplate);

		// Create the ghost pad from template with the target
		GstPad* ghost = gst_ghost_pad_new_from_template(ghostName, targetPad, templ);
		gst_object_unref(targetPad);
		gst_object_unref(templ);

		if (!ghost) return nullptr;

		// Activate and add the pad
		gst_pad_set_active(ghost, TRUE);
		if (!gst_element_add_pad(m_bin, ghost)) {
			gst_object_unref(ghost);
			return nullptr;
		}

		return ghost;
	}

	/**
	 * @brief Creates a "sometimes" (request) source ghost pad on the bin using a pad template.
	 * This allows external elements to request pads from this bin dynamically.
	 * @param ghostName The name of the ghost pad
	 * @param targetElement The internal element whose static pad will be the target
	 * @param targetPadName The name of the target element's static pad
	 * @return The created ghost pad, or nullptr on failure
	 */
	GstPad* makeSometimesSrcGhostPad(const char* ghostName, GstElement* targetElement, const char* targetPadName) {
		if (!m_bin || !targetElement) return nullptr;

		// Get the target pad from the internal element
		GstPad* targetPad = gst_element_get_static_pad(targetElement, targetPadName);
		if (!targetPad) return nullptr;

		// Create a pad template for the sometimes pad
		GstStaticPadTemplate padTemplate = GST_STATIC_PAD_TEMPLATE(
			ghostName,
			GST_PAD_SRC,
			GST_PAD_SOMETIMES,
			GST_STATIC_CAPS_ANY
		);
		GstPadTemplate* templ = gst_static_pad_template_get(&padTemplate);

		// Create the ghost pad from template with the target
		GstPad* ghost = gst_ghost_pad_new_from_template(ghostName, targetPad, templ);
		gst_object_unref(targetPad);
		gst_object_unref(templ);

		if (!ghost) return nullptr;

		// Activate and add the pad
		gst_pad_set_active(ghost, TRUE);
		if (!gst_element_add_pad(m_bin, ghost)) {
			gst_object_unref(ghost);
			return nullptr;
		}

		return ghost;
	}

	/**
	 * @brief Creates a "sometimes" ghost pad without an initial target.
	 * The target can be set later using gst_ghost_pad_set_target().
	 * @param ghostName The name of the ghost pad
	 * @param direction GST_PAD_SINK or GST_PAD_SRC
	 * @return The created ghost pad, or nullptr on failure
	 */
	GstPad* makeSometimesGhostPadNoTarget(const char* ghostName, GstPadDirection direction) {
		if (!m_bin) return nullptr;

		// Create a pad template for the sometimes pad
		GstStaticPadTemplate padTemplate = GST_STATIC_PAD_TEMPLATE(
			ghostName,
			direction,
			GST_PAD_SOMETIMES,
			GST_STATIC_CAPS_ANY
		);
		GstPadTemplate* templ = gst_static_pad_template_get(&padTemplate);

		// Create the ghost pad from template without a target
		GstPad* ghost = gst_ghost_pad_new_no_target_from_template(ghostName, templ);
		gst_object_unref(templ);

		if (!ghost) return nullptr;

		// Activate and add the pad
		gst_pad_set_active(ghost, TRUE);
		if (!gst_element_add_pad(m_bin, ghost)) {
			gst_object_unref(ghost);
			return nullptr;
		}

		return ghost;
	}

	GstElement* m_bin = nullptr;
	Element* m_element = nullptr;
};