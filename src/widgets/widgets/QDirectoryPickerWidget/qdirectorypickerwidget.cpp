#include "qdirectorypickerwidget.h"

QDirectoryPickerWidget::QDirectoryPickerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonSelectDirectory, &QPushButton::clicked, this, &QDirectoryPickerWidget::onSelectDirectoryClicked);
	connect(ui.buttonOpenDirectory, &QPushButton::clicked, this, &QDirectoryPickerWidget::onOpenDirectoryClicked);
	connect(ui.lineDirectory, &QLineEdit::textChanged, this, [this](const QString& newText) {
		this->setSelectedDirectory(QDir(newText));
		});
}

QDirectoryPickerWidget::~QDirectoryPickerWidget() {}

void QDirectoryPickerWidget::onOpenDirectoryClicked()
{
	if (m_selectedDirectory.isEmpty()) {
		QMessageBox::warning(this, tr("No Directory Selected"), tr("Please select a directory first."));
		return;
	}

	QString path = QDir::toNativeSeparators(m_selectedDirectory.path());
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void QDirectoryPickerWidget::onSelectDirectoryClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_selectedDirectory.path());
	if (dir.isEmpty()) {
		return;
	}

	m_selectedDirectory = QDir(dir);
	ui.lineDirectory->setText(m_selectedDirectory.path());
	emit directoryChanged(m_selectedDirectory);
}

void QDirectoryPickerWidget::setSelectedDirectory(const QDir& directory)
{
	m_selectedDirectory = directory;
	ui.lineDirectory->setText(m_selectedDirectory.path());
}

