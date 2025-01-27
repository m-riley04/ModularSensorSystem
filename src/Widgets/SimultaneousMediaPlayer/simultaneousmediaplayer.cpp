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
		mVideoWidgets.push_back(videoWidget);
		QString name = file.fileName();
		ui.tabs->addTab(videoWidget, name);
	}

	// Set current selected media
	pSelectedMediaPlayer = mMediaPlayers.first();

	initWidgets();
	initSignals();
}

SimultaneousMediaPlayer::~SimultaneousMediaPlayer()
{
	mMediaPlayers.clear();
	mVideoWidgets.clear();
}

void SimultaneousMediaPlayer::initWidgets()
{
	// PLAYBACK
	initPlaybackWidgets();
}

void SimultaneousMediaPlayer::initSignals()
{
	/// SELECTION
	connect(ui.tabs, &QTabWidget::currentChanged, [this](int index) {
		pSelectedMediaPlayer = mMediaPlayers[index];
		initPlaybackWidgets();
		});

	/// AUDIO/VOLUME
	connect(ui.sliderVolume, &QSlider::valueChanged, pSelectedMediaPlayer->audioOutput(), &QAudioOutput::setVolume);

	/// PLAYBACK CONTROLS
	connect(ui.buttonPlay, &QPushButton::clicked, [this]() {

		if (pSelectedMediaPlayer->isPlaying()) pSelectedMediaPlayer->pause();
		else pSelectedMediaPlayer->play();

		// Change the button text based on playback state
		ui.buttonPlay->setText(pSelectedMediaPlayer->isPlaying() ? "Pause" : "Play");
		});

	connect(ui.buttonStop, &QPushButton::clicked, pSelectedMediaPlayer, &QMediaPlayer::stop);

	connect(ui.dialSpeed, &QDial::valueChanged, [this]() {

		});

	connect(ui.buttonLoop, &QPushButton::clicked, [this]() {

		});

	connect(ui.buttonSkip, &QPushButton::clicked, [this]() {
		pSelectedMediaPlayer->setPosition(pSelectedMediaPlayer->position() + (SKIP_INTERVAL_S * 1000));
		});

	connect(ui.buttonReverse, &QPushButton::clicked, [this]() {
		pSelectedMediaPlayer->setPosition(pSelectedMediaPlayer->position() - (SKIP_INTERVAL_S * 1000));
		});

	/// SEEK SLIDER
	connect(ui.sliderSeek, &QSlider::valueChanged, pSelectedMediaPlayer, &QMediaPlayer::setPosition);
	connect(pSelectedMediaPlayer, &QMediaPlayer::positionChanged, [this](qint64 pos) {
		// Set position
		ui.sliderSeek->blockSignals(true);
		ui.sliderSeek->setValue(pos);
		ui.sliderSeek->blockSignals(false);


		// Update label
		ui.labelElapsed->setText(QString::number(pos));
		});

	connect(pSelectedMediaPlayer, &QMediaPlayer::durationChanged, [this](qint64 dur) {
		ui.sliderSeek->setMaximum(dur);
		ui.labelDuration->setText(QString::number(dur));
		});
}

void SimultaneousMediaPlayer::initShortcuts()
{
	auto skipShortcut = new QShortcut(QKeySequence::Forward, this);
	auto reverseShortcut = new QShortcut(QKeySequence::Back, this);
	auto playPauseShortcut = new QShortcut(Qt::Key_Space, this);
	auto playShortcut = new QShortcut(Qt::Key_MediaPlay, this);
	auto pauseShortcut = new QShortcut(Qt::Key_MediaPause, this);
	auto playPauseShortcut = new QShortcut(Qt::Key_MediaStop, this);
	auto volumeUpShortcut = new QShortcut(Qt::Key_VolumeUp | Qt::Key_Up, this);
	auto volumeDownShortcut = new QShortcut(Qt::Key_VolumeDown | Qt::Key_Down, this);

}

void SimultaneousMediaPlayer::initPlaybackWidgets()
{
	/// SEEK SLIDER
	//ui.sliderSeek->setEnabled(pSelectedMediaPlayer->isSeekable());
	ui.sliderSeek->setMaximum(pSelectedMediaPlayer->duration());
	ui.sliderSeek->setValue(pSelectedMediaPlayer->position());
	ui.labelDuration->setText(QString::number(pSelectedMediaPlayer->duration()));
	ui.labelElapsed->setText(QString::number(pSelectedMediaPlayer->position()));

	/// BUTTONS
	ui.buttonPlay->setText(pSelectedMediaPlayer->isPlaying() ? "Pause" : "Play");
}
