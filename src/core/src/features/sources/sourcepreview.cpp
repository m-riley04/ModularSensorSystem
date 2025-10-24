#include "features/sources/sourcepreview.h"

SourcePreview::SourcePreview(QUuid sourceId, QObject* parent)
	: QObject(parent), m_sourceId(sourceId)
{}