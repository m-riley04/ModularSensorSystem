#pragma once

#include <QObject>
#include <pipeline/bins/sourcebin.hpp>

class USBAudioSourceBin : public SourceBin {
public:
	USBAudioSourceBin(Element*);

	virtual ~USBAudioSourceBin() = default;

protected:
	virtual bool build() override;
};