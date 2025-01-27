#pragma once

#include <QWidget>
#include <qdialog.h>
#include <qurl.h>
#include <QtMultimedia>
#include <QVideoWidget>
#include "ui_simultaneousmediaplayer.h"

#define SKIP_INTERVAL_S 5

class SimultaneousMediaPlayer : public QDialog
{
	Q_OBJECT

public:
	SimultaneousMediaPlayer(QWidget* parent = nullptr, QStringList mediaFilePaths = {});
	~SimultaneousMediaPlayer();

private:
	Ui::SimultaneousMediaPlayerClass ui;

	QList<QMediaPlayer*> mMediaPlayers;
	QList<QAudioOutput*> mAudioOutputs;
	QList<QVideoWidget*> mVideoWidgets;
	QMediaPlayer* pSelectedMediaPlayer = nullptr;

	void initWidgets();
	void initSignals();
	
	void initShortcuts();
	void initPlaybackWidgets();
};
