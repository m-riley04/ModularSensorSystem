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

		create(nullptr);//element->id().c_str());
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
		GstPad* targetPad = gst_element_get_static_pad(element, padName);
		if (!targetPad) return nullptr;

		GstPad* ghost = gst_ghost_pad_new(ghostName, targetPad);
		gst_object_unref(targetPad);
		if (!ghost) return nullptr;

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