#include "simultaneousmediaplayer.h"
#include <qmessagebox.h>

SimultaneousMediaPlayer::SimultaneousMediaPlayer(QWidget *parent, QStringList mediaFilePaths)
	: QDialog(parent)
{
	ui.setupUi(this);

	// Create widgets for media files
	for (QString filePath : mediaFilePaths) {
		QFileInfo file(filePath);

		// Check if file is good
		if (!file.exists()) {
			QMessageBox::warning(this, "File does not exist", "The file '" + file.fileName() + "' does not exist.");
		}
		auto mediaWidget = new QMediaPlayer(this);
		auto audioOutput = new QAudioOutput(this);
		auto videoWidget = new QVideoWidget(this);
		mediaWidget->setSource(filePath);
		mediaWidget->setAudioOutput(audioOutput);
		mediaWidget->setVideoOutput(videoWidget);
		mMediaPlayers.push_back(mediaWidget);
		mAudioOutputs.push_back(audioOutput);
		mSinkWidgets.push_back(videoWidget);
		QString name = file.fileName();
		ui.playbackGrid->addWidget(videoWidget);
		//ui.tabs->addTab(videoWidget, name);
	}

	// Set current selected media
	pRootMedia = mMediaPlayers.first();

	initWidgets();
	initSignals();
	initShortcuts();
}

SimultaneousMediaPlayer::~SimultaneousMediaPlayer()
{
	mMediaPlayers.clear();
	mSinkWidgets.clear();
}

void SimultaneousMediaPlayer::handleSkip()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->setPosition(pRootMedia->position() + SKIP_INTERVAL_MS);
	}
}

void SimultaneousMediaPlayer::handleReverse()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->setPosition(pRootMedia->position() - SKIP_INTERVAL_MS);
	}
}

void SimultaneousMediaPlayer::handleFrameForward()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->setPosition(pRootMedia->position() + FRAME_INTERVAL_MS);
	}
}

void SimultaneousMediaPlayer::handleFrameBackward()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->setPosition(pRootMedia->position() - FRAME_INTERVAL_MS);
	}
}

void SimultaneousMediaPlayer::handlePlay()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->play();
	}
}

void SimultaneousMediaPlayer::handlePause()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->pause();
	}
}

void SimultaneousMediaPlayer::handleStop()
{
	for (QMediaPlayer* player : mMediaPlayers) {
		player->stop();
	}
}

void SimultaneousMediaPlayer::handlePlayPauseToggle()
{
	if (pRootMedia->isPlaying()) handlePause();
	else handlePlay();

	// Change the button text based on playback state
	ui.buttonPlay->setText(pRootMedia->isPlaying() ? "Pause" : "Play");
}

void SimultaneousMediaPlayer::handleVolumeUp()
{
	ui.sliderVolume->setValue(ui.sliderVolume->value() + 1);
}

void SimultaneousMediaPlayer::handleVolumeDown()
{
	ui.sliderVolume->setValue(ui.sliderVolume->value() - 1);
}

void SimultaneousMediaPlayer::initWidgets()
{
	// PLAYBACK
	initPlaybackWidgets();
}

void SimultaneousMediaPlayer::initSignals()
{
	/// SELECTION
	/*connect(ui.tabs, &QTabWidget::currentChanged, [this](int index) {
		pSelectedMediaPlayer = mMediaPlayers[index];
		initPlaybackWidgets();
		});*/

	/// AUDIO/VOLUME
	connect(ui.sliderVolume, &QSlider::valueChanged, pRootMedia->audioOutput(), &QAudioOutput::setVolume);
	connect(pRootMedia->audioOutput(), &QAudioOutput::volumeChanged, [this](float volume) {
		ui.sliderVolume->blockSignals(true);
		ui.sliderVolume->setValue(volume);
		ui.sliderVolume->blockSignals(false);
		});

	/// PLAYBACK CONTROLS
	connect(ui.buttonPlay, &QPushButton::clicked, this, &SimultaneousMediaPlayer::handlePlayPauseToggle);

	connect(ui.buttonStop, &QPushButton::clicked, this, &SimultaneousMediaPlayer::handleStop);

	connect(ui.dialSpeed, &QDial::valueChanged, [this]() {

		});

	connect(ui.buttonLoop, &QPushButton::clicked, [this]() {

		});

	connect(ui.buttonSkip, &QPushButton::clicked, this, &SimultaneousMediaPlayer::handleSkip);
	connect(ui.buttonReverse, &QPushButton::clicked, this, &SimultaneousMediaPlayer::handleReverse);

	/// SEEK SLIDER
	connect(ui.sliderSeek, &QSlider::valueChanged, [this](int value) {
		for (QMediaPlayer* player : mMediaPlayers) {
			player->setPosition(value);
		}
		});

	connect(pRootMedia, &QMediaPlayer::positionChanged, [this](qint64 pos) {
		// Set position
		ui.sliderSeek->blockSignals(true);
		ui.sliderSeek->setValue(pos);
		ui.sliderSeek->blockSignals(false);

		// Update label
		ui.labelElapsed->setText(QString::number(pos));
		});

	connect(pRootMedia, &QMediaPlayer::durationChanged, [this](qint64 dur) {
		ui.sliderSeek->setMaximum(dur);
		ui.labelDuration->setText(QString::number(dur));
		});
}

void SimultaneousMediaPlayer::initShortcuts()
{
	auto skipShortcut = new QShortcut(Qt::Key_Right, this);
	auto reverseShortcut = new QShortcut(Qt::Key_Left, this);
	auto frameForwardShortcut = new QShortcut(Qt::Key_Period, this);
	auto frameBackwardShortcut = new QShortcut(Qt::Key_Comma, this);
	auto playPauseShortcut = new QShortcut(Qt::Key_Space, this);
	auto playShortcut = new QShortcut(Qt::Key_MediaPlay, this);
	auto pauseShortcut = new QShortcut(Qt::Key_MediaPause, this);
	auto stopShortcut = new QShortcut(Qt::Key_MediaStop, this);
	auto volumeUpShortcut = new QShortcut(Qt::Key_Up, this);
	auto volumeDownShortcut = new QShortcut(Qt::Key_Down, this);

	/// CONNECTIONS
	connect(skipShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleSkip);
	connect(reverseShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleReverse);
	connect(frameForwardShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleFrameForward);
	connect(frameBackwardShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleFrameBackward);
	connect(playPauseShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handlePlayPauseToggle);
	connect(playShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handlePlay);
	connect(pauseShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handlePause);
	connect(stopShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleStop);
	connect(volumeUpShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleVolumeUp);
	connect(volumeDownShortcut, &QShortcut::activated, this, &SimultaneousMediaPlayer::handleVolumeDown);
}

void SimultaneousMediaPlayer::initPlaybackWidgets()
{
	/// SEEK SLIDER
	//ui.sliderSeek->setEnabled(pSelectedMediaPlayer->isSeekable());
	ui.sliderSeek->setMaximum(pRootMedia->duration());
	ui.sliderSeek->setValue(pRootMedia->position());
	ui.labelDuration->setText(QString::number(pRootMedia->duration()));
	ui.labelElapsed->setText(QString::number(pRootMedia->position()));

	/// BUTTONS
	ui.buttonPlay->setText(pRootMedia->isPlaying() ? "Pause" : "Play");

	/// VOLUME
	ui.sliderVolume->setValue(pRootMedia->audioOutput()->volume());
}
