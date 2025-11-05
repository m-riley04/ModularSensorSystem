#pragma once

#include <bins/sourcebin.h>
#include <gst/video/videooverlay.h>
#include <gst/gst.h>

class TestDataSourceBin : public SourceBin {
public:
	TestDataSourceBin(const std::string& id);

	virtual ~TestDataSourceBin() = default;

    // Session pipeline is needed for timestamps (running-time)
    void setSessionPipeline(GstElement* pipeline) {
        m_sessionPipeline = pipeline; // non-owning
    }

    // Push a JSON sample like:
    // {"sensor_id":"random_test","seq":N,"value":0.1234}
    void pushRandomSample(uint64_t seq, double value);

    GstElement* appsrcElement() const { return m_appsrc; } // optional

protected:
	virtual bool build() override;

private:
	GstElement* m_appsrc{ nullptr }; // owned by bin
	GstElement* m_sessionPipeline{ nullptr }; // not owned
};