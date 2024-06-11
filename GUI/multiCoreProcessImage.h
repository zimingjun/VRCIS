#pragma once
#include <QThread>
#include <QFileInfo>
#include <qlist>
#include <gdal_priv.h>
#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <iostream>
class multiCoreProcessImage : public QThread
{
    Q_OBJECT
public:
    multiCoreProcessImage(GDALDataset* poDatasetin, int imageindexin) :poDataset(poDatasetin), imageindex(imageindexin) {};
    ~multiCoreProcessImage() {};
    GDALDataset* poDataset;
    int imageindex;
private:

    void ShowImg(GDALRasterBand* imgBand);
    
    unsigned char* ImgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue);
    
signals:
    void sendMultiProcessedImage(int imageindex, unsigned char* finishBand);

protected:
    void run() override
    {
        ShowImg(poDataset->GetRasterBand(imageindex));
        GDALClose(poDataset);
    }
};