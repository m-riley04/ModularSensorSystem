#pragma once

#include <QObject>
#include <pipeline/bins/sourcebin.hpp>

class ArduinoPanTiltMountBin : public SourceBin {
public:
	ArduinoPanTiltMountBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~ArduinoPanTiltMountBin() = default;

protected:
	virtual bool build() override;
};