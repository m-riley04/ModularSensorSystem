#pragma once

#include <gst/video/videooverlay.h>
#include <gst/gst.h>
#include <bins/sourcebin.hpp>
#include <utils/boost_qt_conversions.hpp>

class TestDataSourceBin : public SourceBin {
public:
	TestDataSourceBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~TestDataSourceBin() = default;

    // Push a JSON sample like:
    // {"sensor_id":"random_test","seq":N,"value":0.1234}
    void pushRandomSample(uint64_t seq, double value);

    GstElement* appsrcElement() const { return m_appsrc; } // optional

protected:
	virtual bool build() override;

private:
	GstElement* m_appsrc{ nullptr }; // owned by bin
};