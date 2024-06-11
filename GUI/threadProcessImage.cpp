#include "threadProcessImage.h"

unsigned char* ProcessImage::ImgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue)
{
    unsigned char* resBuffer = new unsigned char[bandSize];
    double max, min;
    double minmax[2];


    currentBand->ComputeRasterMinMax(1, minmax);
    min = minmax[0];
    max = minmax[1];
    //min = minmax[0] + 0.01*(minmax[1]- minmax[0]);
    //max = minmax[1] - 0.01 *(minmax[1] - minmax[0]);
    //1%线性拉伸图像功能
    if (min <= noValue && noValue <= max)
    {
        min = 0;
    }
#pragma omp parallel for
    for (int i = 0; i < bandSize; i++)
    {
        if (buffer[i] > max)
        {
            resBuffer[i] = 255;
        }
        else if (buffer[i] <= max && buffer[i] >= min)
        {
            resBuffer[i] = static_cast<uchar>(255 - 255 * (max - buffer[i]) / (max - min));
        }
        else
        {
            resBuffer[i] = 0;
        }
    }

    return resBuffer;
}

void ProcessImage::ShowBand(GDALRasterBand* band)
{
    if (band == NULL)
    {
        return;
    }

    QList<GDALRasterBand*> myBand;
    myBand.append(band);
    myBand.append(band);
    myBand.append(band);

    ShowImg(&myBand);

}

void ProcessImage::ShowImg(QList<GDALRasterBand*>* imgBand)
{

    if (imgBand->size() != 3)
    {
        return;
    }

    int imgWidth = imgBand->at(0)->GetXSize();
    int imgHeight = imgBand->at(0)->GetYSize();
    //金字塔加载模型
    //m_scaleFactor = this->height() * 1.0 / imgHeight;
    //int iScaleWidth = ( int )( imgWidth * m_scaleFactor - 1 );
    //int iScaleHeight = ( int )( imgHeight * m_scaleFactor - 1 );
    int iScaleWidth = (int)(imgWidth - 1);
    int iScaleHeight = (int)(imgHeight - 1);
    GDALDataType dataType = imgBand->at(0)->GetRasterDataType();
    if (dataType == GDT_Byte)
    {
        unsigned char* rBand = new unsigned char[iScaleWidth * iScaleHeight];
        unsigned char* gBand = new unsigned char[iScaleWidth * iScaleHeight];
        unsigned char* bBand = new unsigned char[iScaleWidth * iScaleHeight];



        if (m_showColor == true)
        {
            imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Byte, 0, 0);
            emit sendProcessBarNumber(25);
            imgBand->at(1)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, gBand, iScaleWidth, iScaleHeight, GDT_Byte, 0, 0);
            emit sendProcessBarNumber(50);
            imgBand->at(2)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, bBand, iScaleWidth, iScaleHeight, GDT_Byte, 0, 0);
            emit sendProcessBarNumber(75);


        }
        else
        {
            imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Byte, 0, 0);
            emit sendProcessBarNumber(50);

            gBand = rBand;
            bBand = rBand;
        }

        long long bytePerLine = (long long)(iScaleWidth * 24 + 31) / 8;
        long long testdata = bytePerLine * iScaleHeight * 3;
        unsigned char* allBandUC = new unsigned char[testdata];
        //unsigned char* allBandUC = new unsigned char[bytePerLine * iScaleHeight * 3];
#pragma omp parallel for
        for (int h = 0; h < iScaleHeight; h++)
        {
            for (int w = 0; w < iScaleWidth; w++)
            {
                allBandUC[h * bytePerLine + w * 3 + 0] = rBand[h * iScaleWidth + w];
                allBandUC[h * bytePerLine + w * 3 + 1] = gBand[h * iScaleWidth + w];
                allBandUC[h * bytePerLine + w * 3 + 2] = bBand[h * iScaleWidth + w];
            }
        }
        emit sendProcessBarNumber(90);
        if (m_showColor == true)
        {

            delete[]rBand;
            delete[]gBand;
            delete[]bBand;

        }
        else
        {
            delete[]rBand;

        }
        QGraphicsPixmapItem* imgItem = new QGraphicsPixmapItem;
        imgItem->setPixmap(QPixmap::fromImage(QImage(allBandUC, iScaleWidth, iScaleHeight, bytePerLine, QImage::Format_RGB888)));


        emit sendProcessedImage(imgItem);

        delete[]allBandUC;

    }
    else
    {
        float* rBand = new float[iScaleWidth * iScaleHeight];
        float* gBand = new float[iScaleWidth * iScaleHeight];
        float* bBand = new float[iScaleWidth * iScaleHeight];

        unsigned char* rBandUC, * gBandUC, * bBandUC;

        if (m_showColor == true)
        {
            imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
            emit sendProcessBarNumber(10);
            imgBand->at(1)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, gBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
            emit sendProcessBarNumber(25);
            imgBand->at(2)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, bBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
            emit sendProcessBarNumber(50);


            rBandUC = ImgSketch(rBand, imgBand->at(0), iScaleWidth * iScaleHeight, imgBand->at(0)->GetNoDataValue());
            emit sendProcessBarNumber(60);
            gBandUC = ImgSketch(gBand, imgBand->at(1), iScaleWidth * iScaleHeight, imgBand->at(1)->GetNoDataValue());
            emit sendProcessBarNumber(70);
            bBandUC = ImgSketch(bBand, imgBand->at(2), iScaleWidth * iScaleHeight, imgBand->at(2)->GetNoDataValue());
            emit sendProcessBarNumber(80);
        }
        else
        {
            imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
            emit sendProcessBarNumber(10);
            rBandUC = ImgSketch(rBand, imgBand->at(0), iScaleWidth * iScaleHeight, imgBand->at(0)->GetNoDataValue());
            emit sendProcessBarNumber(20);
            gBandUC = rBandUC;
            bBandUC = rBandUC;
        }

        delete[]rBand;
        delete[]gBand;
        delete[]bBand;

        long long bytePerLine = (long long)(iScaleWidth * 24 + 31) / 8;
        long long testdata = bytePerLine * iScaleHeight * 3;
        unsigned char* allBandUC = new unsigned char[testdata];
        //unsigned char* allBandUC = new unsigned char[bytePerLine * iScaleHeight * 3];
#pragma omp parallel for
        for (int h = 0; h < iScaleHeight; h++)
        {
            for (int w = 0; w < iScaleWidth; w++)
            {
                allBandUC[h * bytePerLine + w * 3 + 0] = rBandUC[h * iScaleWidth + w];
                allBandUC[h * bytePerLine + w * 3 + 1] = gBandUC[h * iScaleWidth + w];
                allBandUC[h * bytePerLine + w * 3 + 2] = bBandUC[h * iScaleWidth + w];
            }
        }
        emit sendProcessBarNumber(90);
        if (m_showColor == true)
        {
            delete[]bBandUC;
            delete[]rBandUC;
            delete[]gBandUC;
        }
        else
        {
            delete[]bBandUC;
        }
        QGraphicsPixmapItem* imgItem = new QGraphicsPixmapItem;
        imgItem->setPixmap(QPixmap::fromImage(QImage(allBandUC, iScaleWidth, iScaleHeight, bytePerLine, QImage::Format_RGB888)));


        emit sendProcessedImage(imgItem);

        delete[]allBandUC;

    }
    emit finishProcessImage();
}
void ProcessImage::run() 
{


    emit startProcessImage();
    QFileInfo fileInfo(imgPath);
    //std::cout << imgPath.toStdString().c_str();
    const QString filenamenow = fileInfo.fileName();

    poDataset = (GDALDataset*)GDALOpen(imgPath.toStdString().c_str(), GA_ReadOnly);

    if (poDataset == NULL)
    {
        //QMessageBox::critical(this, tr("Error!"), tr("Can not open file %1").arg(imgPath));
        return;
    }

    int a = poDataset->GetRasterCount();
    if ( a == 1)
    {
        m_showColor = false;
        ShowBand(poDataset->GetRasterBand(1));
        emit sendProcessBarNumber(100);
        emit finishProcessImage();
        GDALClose(poDataset);
    }

    else
    {

        m_showColor = true;
        imgWidth = poDataset->GetRasterBand(1)->GetXSize();
        imgHeight = poDataset->GetRasterBand(1)->GetYSize();
        if (imgWidth > 10000)
        {
            emit sendProcessBarNumber(25);
            //多核加载接口，快了30%，仅在加载大图时使用
            poDataset1 = (GDALDataset*)GDALOpen(imgPath.toStdString().c_str(), GA_ReadOnly);
            poDataset2 = (GDALDataset*)GDALOpen(imgPath.toStdString().c_str(), GA_ReadOnly);


            multiCoreProcessImage* r = new multiCoreProcessImage(poDataset, 1);
            multiCoreProcessImage* g = new multiCoreProcessImage(poDataset1, 2);
            multiCoreProcessImage* b = new multiCoreProcessImage(poDataset2, 3);
            connect(r, &multiCoreProcessImage::sendMultiProcessedImage, this, &ProcessImage::receiveMultiProcessedImage);
            connect(g, &multiCoreProcessImage::sendMultiProcessedImage, this, &ProcessImage::receiveMultiProcessedImage);
            connect(b, &multiCoreProcessImage::sendMultiProcessedImage, this, &ProcessImage::receiveMultiProcessedImage);
            r->start();
            g->start();
            b->start();
            r->wait();
            g->wait();
            b->wait();
            while (finishNumber == 3)
            {
                delete r;
                delete g;
                delete b;
                int iScaleWidth = (int)(imgWidth - 1);
                int iScaleHeight = (int)(imgHeight - 1);
                int iScaleWidth1 = (int)(imgWidth - 1);
                int iScaleHeight1 = (int)(imgHeight - 1);
                if (iScaleWidth > 32000)iScaleWidth = 23000;
                if (iScaleHeight > 32000)iScaleHeight = 23000;
                auto bytePerLine = (long long)(iScaleWidth * 24 + 31) / 8;
                auto testdata = bytePerLine * iScaleHeight * 3;
                emit sendProcessBarNumber(70);
                auto* allBandUC = new unsigned char[testdata];
                
#pragma omp parallel for
                for (int h = 0; h < iScaleHeight; h++)
                {
                    for (int w = 0; w < iScaleWidth; w++)
                    {
                        allBandUC[h * bytePerLine + w * 3 + 0] = finishRBand[h * iScaleWidth1 + w];
                        allBandUC[h * bytePerLine + w * 3 + 1] = finishGBand[h * iScaleWidth1 + w];
                        allBandUC[h * bytePerLine + w * 3 + 2] = finishBBand[h * iScaleWidth1 + w];
                    }
                }
                delete[]finishRBand;
                delete[]finishGBand;
                delete[]finishBBand;
                emit sendProcessBarNumber(90);
                QGraphicsPixmapItem* imgItem = new QGraphicsPixmapItem;
                imgItem->setPixmap(QPixmap::fromImage(QImage(allBandUC, iScaleWidth, iScaleHeight, bytePerLine, QImage::Format_RGB888)));
                emit sendProcessedImage(imgItem);
                delete[]allBandUC;
                finishNumber = 0;
                imgWidth = 0;
                imgHeight = 0;
                emit finishProcessImage();

            }
        }
        else
        {
            //单线程接口

            QList<GDALRasterBand*> bandList;
            bandList.append(poDataset->GetRasterBand(1));
            bandList.append(poDataset->GetRasterBand(2));
            bandList.append(poDataset->GetRasterBand(3));
            ShowImg(&bandList);
            GDALClose(poDataset);
        }


    }


}
void  ProcessImage::receiveMultiProcessedImage(int imageindex, unsigned char* finishBand)
{
    if (imageindex == 1)
    {
        (finishNumber)++;
        finishRBand = finishBand;
    }
    if (imageindex == 2)
    {
        (finishNumber)++;
        finishGBand = finishBand;
    }
    if (imageindex == 3)
    {
        (finishNumber)++;
        finishBBand = finishBand;
    }


}