#pragma once

#include <QWidget>
#include <qdialog.h>
#include <qurl.h>
#include <QtMultimedia>
#include <QVideoWidget>
#include "ui_simultaneousmediaplayer.h"

#define SKIP_INTERVAL_MS 5000
#define FRAME_INTERVAL_MS 10

class SimultaneousMediaPlayer : public QDialog
{
	Q_OBJECT

public:
	SimultaneousMediaPlayer(QWidget* parent = nullptr, QStringList mediaFilePaths = {});
	~SimultaneousMediaPlayer();

public slots:
	void handleSkip();
	void handleReverse();
	void handleFrameForward();
	void handleFrameBackward();
	void handlePlay();
	void handlePause();
	void handleStop();
	void handlePlayPauseToggle();
	void handleVolumeUp();
	void handleVolumeDown();

private:
	Ui::SimultaneousMediaPlayerClass ui;

	QList<QMediaPlayer*> mMediaPlayers;
	QList<QAudioOutput*> mAudioOutputs;
	QList<QVideoWidget*> mVideoWidgets;
	QMediaPlayer* pRootMedia = nullptr;

	void initWidgets();
	void initSignals();
	
	void initShortcuts();
	void initPlaybackWidgets();
};
