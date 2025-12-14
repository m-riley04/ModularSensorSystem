#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/bins/sourcebin.hpp>
#include <QJsonDocument>

class ArduinoPanTiltMountBin : public SourceBin {
public:
	ArduinoPanTiltMountBin(const boost::uuids::uuid& uuid, const std::string& id);
	virtual ~ArduinoPanTiltMountBin() = default;

	GstElement* appsrcElement() const { return m_appsrc; }
	void pushSample(QByteArray json);


protected:
	virtual bool build() override;

private:
	GstElement* m_appsrc{ nullptr }; // owned by bin
};