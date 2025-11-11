#pragma once

#include <QObject>
#include <QDebug>
#include <pipeline/bins/sourcebin.hpp>

class USBAudioSourceBin : public SourceBin {
public:
	USBAudioSourceBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~USBAudioSourceBin() = default;

protected:
	virtual bool build() override;
};