#include "sourcepreview.h"

SourcePreview::SourcePreview(QObject* parent)
	: QObject(parent), pSource(static_cast<Source*>(parent)) // TODO: MAKE THIS SAFER/ERROR HANDLE
{}

SourcePreview::~SourcePreview()
{}
