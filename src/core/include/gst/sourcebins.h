#pragma once
#include <features/sources/source.h>

class BinBase {
public:
	BinBase();
};

class SourceBin : public BinBase {
public:
	SourceBin(Source* source);
	~SourceBin();

};
