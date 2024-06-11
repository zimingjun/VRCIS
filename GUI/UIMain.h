#pragma once
#ifndef IMGTEST_H
#define IMGTEST_H
#include "mosaic1.h"
#include "sam.h"
#include "ChangeDetect.h"
#include "fusion.h"
#include "sarPreprocessing.h"
#include "IRGS.h"
#include "sgementDialog.h"
#include "toolBoxTreeView.h"
#include "MessageDialog.h"
#include "preprocessingWindows.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <atomic>
#include <thread>

#include <vector>
#include <string>
#include "ui_Main.h"
#include <QProgressBar>
#include <QLabel>
#include <QPropertyAnimation>
#include <QEventLoop>
#undef slots
#include "torch/torch.h"
#include <torch/script.h>
#define slots Q_SLOTS

class MapCanvas;


class UIImgTest : public QMainWindow
{
    Q_OBJECT
    
public:
    UIImgTest( QWidget *parent = 0 );
    ~UIImgTest();
    
    Mosaic* thread;
    CD* cdthread;
    PcaFusion* fusionthread;
    sarPreprocessing* sarPreprocessingthread;
    segmentstart* segmentThread;
    ToolBoxItemDelegate* toolBoxdelegate;
    QProgressBar* getpProgressBar() {return pProgressBar;};
    QLabel* getpLabel(){ return pLabel; };
    QProgressBar* pProgressBar;
    QLabel* pLabel;
    QPropertyAnimation* pAnimation;
    PreprocessingDialog* Preprocessing;
    SegmentDialog* segmentdialog;
public slots:
    //主界面类
    void PickOpenFile();
    void Exit();
    void ShowFileListWindow();
    void ShowInforWindow();
    void CloseImg();
    
    //功能类
    void openMosaicWindows();
    void openFusionWindows();
    void openChangedetectWindows(); 
    void openPreprocessingWindows();
    void samWindows();
    void sar2opticalWindows();
    void openSegmentMRFWindows();
    void openSegmentClusterWindows();
    
private:
   
    Ui::ImgTestClass ui;
    
    MapCanvas *myMap;

private slots:
    

    void startTask();

    void handleFatherLayerNumber(int itemNumber, const QString fatherLayerPath);
    void handlDeleteLayerNumber(int itemNumber, const QString fatherLayerPath);
    void handleImageMosaicdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void finishImageMosaicSlot(const QString ImageMasaicPath);
    void receiveStatueBarMessage(QString Message);
    void receiveProcessBarNumber(int number);
    void handleImageFusiondata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void finishImageFusionSlot(const QString ImageMasaicPath);
    void handleImageChangeDetectdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void handleImageSegmentdata(const QString ImageFusionPath1,  const QString ImageFusionSavePath, const QString ImageFusionMethod);
    void finishImageSegmentSlot(const QString ImageMasaicPath);
    void finishImageChangeDetectSlot(const QString ImageMasaicPath);
    void handleImagePreprocessingDialogdata(const QString ImagePath, const QString ImagexmlPath, const QString ImageSavePath);
    void finishImagePreprocessingSlot(const QString ImageMasaicPath);
};

#endif // IMGTEST_H
