#include <pipeline/branches/previewbranch.hpp>

PreviewBranch::PreviewBranch(boost::uuids::uuid& uuid, std::string& id)
	: TeeBranch(uuid, id)
{
	// Add compositor element to the body of the branch
	GstElement* compositor = gst_element_factory_make("compositor", ("preview_compositor_" + boost::uuids::to_string(uuid)).c_str());
	if (!compositor) {
		// Failed to create compositor
		return;
	} 
	g_object_set(compositor, "background", 3, nullptr);
	if (!gst_bin_add(GST_BIN(this->bin()), compositor)) {
		// Failed to add compositor to bin
		gst_object_unref(compositor);
		return;
	}

	// Link prefix bin to compositor
	if (!gst_element_link(this->prefix().bin(), compositor)) {
		// Failed to link prefix to compositor
		gst_bin_remove(GST_BIN(this->bin()), compositor);
		return;
	}

}

PreviewBranch::~PreviewBranch()
{
}
