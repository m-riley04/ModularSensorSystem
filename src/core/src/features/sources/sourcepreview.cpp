#include "features/sources/sourcepreview.h"

SourcePreview::SourcePreview(QObject* parent)
	: QObject(parent), m_source(static_cast<Source*>(parent)) // TODO: MAKE THIS SAFER/ERROR HANDLE
{}