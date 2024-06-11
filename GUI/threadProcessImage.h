#pragma once
#include <QThread>
#include <QTimer>
#include <omp.h>
#include <QFileInfo>
#include <qlist>
#include <Windows.h>
#include <gdal_priv.h>
#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <iostream>
#include "multiCoreProcessImage.h"

class ProcessImage : public QThread
{
    Q_OBJECT

public:
    ProcessImage() {
        finishNumber = 0;
        
    };
    ~ProcessImage() {};
    GDALDataset* poDataset;
    GDALDataset* poDataset1;
    GDALDataset* poDataset2;
    unsigned char* ImgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue);
    
    void setProcessImage(QString imagePath){ imgPath = imagePath;}
    void ShowBand(GDALRasterBand* band);
    

    void ShowImg(QList<GDALRasterBand*>* imgBand);
    

public slots:
    void startProcessImageSlot()
    {
        emit startProcessImage();
    };
    void finishProcessImageSlot() 
    {
        emit finishProcessImage();
    };
    void receiveProcessBarNumber(int number)
    {
        emit sendProcessBarNumber(number);
    };
    void receiveMultiProcessedImage(int imageindex, unsigned char* finishBand);
    
public:
     unsigned char* finishRBand;
     unsigned char* finishGBand;
     unsigned char* finishBBand;

signals:
    void startProcessImage();
    void finishProcessImage();
    void sendProcessedImage(QGraphicsPixmapItem *imgItem);
    void sendProcessBarNumber(int number);
    
private:
    QString imgPath;
    int imageProcessIndex;
    int imageProcessTotalNumber;
    bool m_showColor;
    int finishNumber;
    int imgWidth ;
    int imgHeight;
    
protected:
    void run() override;
    
};

