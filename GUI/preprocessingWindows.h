#pragma once
#include <QDialog>
#include <QPropertyAnimation>
#include <QLabel>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <qpushbutton>
#include <QFileDialog>
#include <QMessageBox>

class PreprocessingDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PreprocessingDialog(QWidget* parent = nullptr);

private slots:
    void chooseSaveLocationClicked();
    void selectClicked(); 
    void chooseOpenLocationXmlClicked();
    void chooseOpenLocationTifClicked();
private:
    QHBoxLayout* comboLayout;
    QVBoxLayout* layout;
    QPushButton* m_chooseButton;
    QLabel* m_fileLabel;
    QPushButton* m_chooseButton1;
    QLabel* m_fileLabel1;
    QPushButton* m_chooseButton2;
    QLabel* m_fileLabel2;
    QPushButton* m_selectButton;
signals:
    void sendImagePreprocessingdata(const QString ImagePath, const QString ImagexmlPath, const QString ImageSavePath);

};