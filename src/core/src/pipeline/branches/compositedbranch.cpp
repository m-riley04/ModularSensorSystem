#include "pipeline/branches/compositedbranch.hpp"

CompositedBranch::CompositedBranch(boost::uuids::uuid& uuid, std::string& id)
	: TeeBranch(uuid, id)
{
	// Create compositor element
	m_compositor = gst_element_factory_make("compositor", "compositor");
	if (!m_compositor) {
		// Failed to create compositor
		return;
	}
	g_object_set(m_compositor, "background", 3, nullptr);
	// Add compositor to branch bin
	if (!gst_bin_add(GST_BIN(this->bin()), m_compositor)) {
		// Failed to add compositor to bin
		gst_object_unref(m_compositor);
		m_compositor = nullptr;
		return;
	}
	// Link compositor to prefix bin
	if (!gst_element_link(m_compositor, this->prefix().bin())) {
		// Failed to link compositor to prefix
		gst_bin_remove(GST_BIN(this->bin()), m_compositor);
		m_compositor = nullptr;
		return;
	}
}

CompositedBranch::~CompositedBranch()
{
	if (m_compositor) {
		gst_element_set_state(m_compositor, GST_STATE_NULL);
		gst_bin_remove(GST_BIN(this->bin()), m_compositor);
		gst_object_unref(m_compositor);
		m_compositor = nullptr;
	}
}
