#pragma once

#include <QObject>
#include <QDebug>
#include "features/sources/IClippableSource/ClipBufferBase/clipbufferbase.h"
#include <features/sources/Source/SourcePropertiesWidget/sourcepropertieswidget.h>
#include <features/sources/IClippableSource/IClippableSource.h>
#include <features/sources/Source/SourceError/sourceerror.h>
#include "controllers/BackendControllerBase/backendcontrollerbase.h"

class ClipController : public BackendControllerBase
{
	Q_OBJECT

public:
	enum State {
		IDLE,
		CLIPPING
	};

public:
	ClipController(QString clippingDir, QObject *parent = nullptr);
	~ClipController();

	State state() const { return mState; }

public slots:
	/// <summary>
	/// Clips MULTIPLE sources and saves the clips to the specified directory.
	/// </summary>
	/// <param name="sources"></param>
	/// <param name="dirPath"></param>
	void clip(QList<Source*> sources, QString dirPath = QString());

	/// <summary>
	/// Clips SINGLE source and saves the clip to the specified directory.
	/// </summary>
	/// <param name="source"></param>
	/// <param name="dirPath"></param>
	void clip(Source* source, QString dirPath = QString());

	/// <summary>
	/// Flushes the clip buffers of ALL passed sources
	/// </summary>
	void flush(QList<Source*> sources);

	/// <summary>
	/// Flushes the clip buffer of the SINGLE source
	/// </summary>
	void flush(Source* source);

	void addClipBuffer(ClipBufferBase* buffer);
	void removeClipBuffer(ClipBufferBase* buffer);

private:
	State mState = State::IDLE;
	QString mClippingDir;
	QList<ClipBufferBase*> mClipBuffers;

signals:
	void clipBufferAdded(ClipBufferBase* buffer);
	void clipBufferRemoved(ClipBufferBase* buffer);
	void flushed(ClipBufferBase* buffer);
	void clipSaved(const QString& fileName);

	void errorOccurred(const SourceError& e);
};
