#include "processorbase.h"

ProcessorBase::ProcessorBase(Source* source, QObject* parent)
	: QObject(parent), pSource(source)
{}