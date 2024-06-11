#include "multiCoreProcessImage.h"

void multiCoreProcessImage::ShowImg(GDALRasterBand* imgBand)
{


    int imgWidth = imgBand->GetXSize();
    int imgHeight = imgBand->GetYSize();
    int iScaleWidth = (int)(imgWidth - 1);
    int iScaleHeight = (int)(imgHeight - 1);
    GDALDataType dataType = imgBand->GetRasterDataType();
    if (dataType == GDT_Byte)
    {
        unsigned char* rBand = new unsigned char[iScaleWidth * iScaleHeight];
        imgBand->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Byte, 0, 0);



        emit sendMultiProcessedImage(imageindex, rBand);
        //GDALClose(poDataset);


    }
    else
    {
        float* rBandFloat = new float[iScaleWidth * iScaleHeight];
        unsigned char* rBandUC;
        imgBand->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBandFloat, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
        rBandUC = ImgSketch(rBandFloat, imgBand, iScaleWidth * iScaleHeight, imgBand->GetNoDataValue());
        delete[]rBandFloat;


        emit sendMultiProcessedImage(imageindex, rBandUC);
        //GDALClose(poDataset);
    }
}

unsigned char* multiCoreProcessImage::ImgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue)
{
    unsigned char* resBuffer = new unsigned char[bandSize];
    double max, min;
    double minmax[2];


    currentBand->ComputeRasterMinMax(1, minmax);
    min = minmax[0];
    max = minmax[1];
    //min = minmax[0] + 0.01 * (minmax[1] - minmax[0]);
    //max = minmax[1] - 0.01 * (minmax[1] - minmax[0]);
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