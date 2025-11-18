#pragma once

#include <QObject>
#include <QDebug>
#include <pipeline/bins/recorderbin.hpp>
#include <gst/video/videooverlay.h>

class USBVideoSourceRecorderBin : public RecorderBin {
public:
	USBVideoSourceRecorderBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~USBVideoSourceRecorderBin() = default;

	GstElement* valveElement() const { return m_valveElement; }

protected:
	virtual bool build() override;

private:
	GstElement* m_valveElement = nullptr;
};