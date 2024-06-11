#include "preprocessingWindows.h"

PreprocessingDialog::PreprocessingDialog(QWidget* parent)
{
	setWindowTitle("SAR Preprocessing");
	resize(1000, 500);
	QVBoxLayout* layout = new QVBoxLayout(this);
	QHBoxLayout* fileLayout = new QHBoxLayout;

	m_chooseButton = new QPushButton("chose path", this);
	connect(m_chooseButton, &QPushButton::clicked, this, &PreprocessingDialog::chooseOpenLocationTifClicked);

	m_fileLabel = new QLabel(this);
	m_fileLabel->setText("preprocessingImagePath:");

	fileLayout->addWidget(m_chooseButton);
	fileLayout->addWidget(m_fileLabel);

	layout->addLayout(fileLayout);


	QHBoxLayout* fileLayout1 = new QHBoxLayout;
	m_chooseButton1 = new QPushButton("chose path", this);
	connect(m_chooseButton1, &QPushButton::clicked, this, &PreprocessingDialog::chooseOpenLocationXmlClicked);

	m_fileLabel1 = new QLabel(this);
	m_fileLabel1->setText("calibrationXmlPath:");

	fileLayout1->addWidget(m_chooseButton1);
	fileLayout1->addWidget(m_fileLabel1);

	layout->addLayout(fileLayout1);

	QHBoxLayout* fileLayout2 = new QHBoxLayout;
	m_chooseButton2 = new QPushButton("chose save path", this);
	connect(m_chooseButton2, &QPushButton::clicked, this, &PreprocessingDialog::chooseSaveLocationClicked);

	m_fileLabel2 = new QLabel(this);
	m_fileLabel2->setText("savePath:");

	fileLayout2->addWidget(m_chooseButton2);
	fileLayout2->addWidget(m_fileLabel2);

	layout->addLayout(fileLayout2);

	m_selectButton = new QPushButton("OK", this);
	m_selectButton->setEnabled(false);
	connect(m_selectButton, &QPushButton::clicked, this, &PreprocessingDialog::selectClicked);

	layout->addWidget(m_selectButton);
}
void PreprocessingDialog::selectClicked()
{
	QString savepathText = m_fileLabel2->text();
	QString imgpathText = m_fileLabel->text();
	QString xmlpathText = m_fileLabel1->text();
	if (m_fileLabel2->text().isEmpty() || m_fileLabel->text().isEmpty() || m_fileLabel1->text().isEmpty()) {
		QMessageBox::warning(this, "error", "please set savepath");
		return;
	}

	close();
	emit sendImagePreprocessingdata(imgpathText, xmlpathText, savepathText);
}
void PreprocessingDialog::chooseOpenLocationXmlClicked()
{
	QString saveLocation = QFileDialog::getOpenFileName(this, "chose  path", QString(), "XML FILE (*.xml)");
	if (!saveLocation.isEmpty()) {
		m_fileLabel1->setText(saveLocation);
		if (!m_fileLabel2->text().isEmpty() || !m_fileLabel->text().isEmpty() || !m_fileLabel1->text().isEmpty()) {
			m_selectButton->setEnabled(true);
		}

	}
}
void PreprocessingDialog::chooseOpenLocationTifClicked()
{
	QString saveLocation = QFileDialog::getOpenFileName(this, "chose  path", QString(), "IMAGE FILE (*.tiff *.jpg *.tif)");
	if (!saveLocation.isEmpty()) {
		m_fileLabel->setText(saveLocation);
		if (!m_fileLabel2->text().isEmpty() || !m_fileLabel->text().isEmpty() || !m_fileLabel1->text().isEmpty()) {
			m_selectButton->setEnabled(true);
		}
	}
}
void PreprocessingDialog::chooseSaveLocationClicked()
{
	QString saveLocation = QFileDialog::getSaveFileName(this, "chose save path", QString(), "IMAGE FILE (*.tif *.jpg)");
	if (!saveLocation.isEmpty()) {
		m_fileLabel2->setText(saveLocation);
		if (!m_fileLabel2->text().isEmpty() || !m_fileLabel->text().isEmpty() || !m_fileLabel1->text().isEmpty()) {
			m_selectButton->setEnabled(true);
		}
	}

}