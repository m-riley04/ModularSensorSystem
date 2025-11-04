#pragma once
#include <features/sources/source.h>
#include <gst/gst.h>

class BinBase {
public:
	virtual ~BinBase() {
		if (m_bin) {
			// Bin should already be removed from any parent and set to NULL state.
			gst_object_unref(m_bin);
			m_bin = nullptr;
		}
	}

	GstElement* bin() const { return m_bin; }

protected:
	explicit BinBase(std::string name) : m_name(std::move(name))
	{
		createBin(m_name.c_str());
	}

	// Helper: create bin with a given name
	bool createBin(const char* name) {
		m_bin = gst_bin_new(name);
		return m_bin != nullptr;
	}

	// Helper: add multiple elements to bin
	void addToBin(std::initializer_list<GstElement*> elems) {
		for (auto* e : elems) {
			if (e) gst_bin_add(GST_BIN(m_bin), e);
		}
	}

	// Helper: create a ghost pad from an element’s static pad
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
	std::string m_name;
};

class SourceBin : public BinBase {
public:
	SourceBin(Source* source);
	~SourceBin();

};
