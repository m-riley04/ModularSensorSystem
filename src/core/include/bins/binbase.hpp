#pragma once
#include <gst/gst.h>
#include <string>

class BinBase {
public:
	virtual ~BinBase() = default;

	GstElement* bin() const { return m_bin; }

protected:
	explicit BinBase(const boost::uuids::uuid& uuid, const std::string& id) : m_uuid(uuid), m_id(id)
	{
		create(m_id.c_str());
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

	GstElement* m_bin = nullptr;
	boost::uuids::uuid m_uuid;
	std::string m_id;
};