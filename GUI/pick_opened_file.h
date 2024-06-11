#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
class ParentClassDialog : public QDialog {
    Q_OBJECT

public:
    enum toolBox{imageMosaic = 1,imageFusion,imageChangeDetect,imageMRFSegment, imageClusterSegment,sar2optical
    };
    explicit ParentClassDialog(QStandardItemModel* model, QWidget* parent = nullptr);
    QStringList convertStandardItemModelToStringList(QStandardItemModel* model);
    void setFileListModel(QStandardItemModel* model);
    void setToolChoice(int toolUserChoice);
    
private slots:
    void chooseSaveLocationClicked();
    void selectClicked();
private:
    QHBoxLayout* comboLayout;
    QVBoxLayout* layout;
    QStandardItemModel* m_model;
    QStringListModel* m_listModel;
    QListView* m_listView;
    QPushButton* m_chooseButton;
    QLabel* m_fileLabel;
    QComboBox* m_combo;
    QPushButton* m_selectButton;
    int toolChoice;
    bool InitializeSelection;
    bool segmentMethod;
    

signals:
    void sendImageFusiondata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void sendImageMosaicdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void sendImageChangeDetectdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath,const QString ImageFusionMethod);
    void sendImageSegmentdata(const QString ImageFusionPath1, const QString ImageFusionSavePath, const QString ImageFusionMethod);
};