#include "qdirectorypickerwidget.h"

QDirectoryPickerWidget::QDirectoryPickerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonSelectDirectory, &QPushButton::clicked, this, &QDirectoryPickerWidget::onSelectDirectoryClicked);
	connect(ui.buttonOpenDirectory, &QPushButton::clicked, this, &QDirectoryPickerWidget::onOpenDirectoryClicked);
	/*connect(ui.lineDirectory, &QLineEdit::textChanged, this, [this](const QString& newText) {
		this->setSelectedDirectory(QDir(newText));
		});*/
}

QDirectoryPickerWidget::~QDirectoryPickerWidget() {}

void QDirectoryPickerWidget::onOpenDirectoryClicked()
{
	if (!m_selectedDirectory.exists()) {
		QMessageBox::warning(this, tr("Directory Does Not Exist"), tr("Please select a directory that exists."));
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

	setSelectedDirectory(QDir(dir));
}

void QDirectoryPickerWidget::setSelectedDirectory(const QDir& directory)
{
	m_selectedDirectory = QDir(directory);
	ui.lineDirectory->setText(m_selectedDirectory.path());
	emit directoryChanged(m_selectedDirectory);
}

