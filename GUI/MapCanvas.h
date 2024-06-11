#pragma once
#ifndef MAPCANVAS_H
#define MAPCANVAS_H
#include <iostream>
#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include <QStandardItemModel>
#include <vector>
#include <gdal_priv.h>
#include "ui_Main.h"
#include "filetreeview.h"
#include "pick_opened_file.h"
#include "threadProcessImage.h"
#include "ImageStructure.h"
class FileListDelegate;
//class ParentClassDialog;

class MapCanvas : public QGraphicsView
{
    Q_OBJECT
    
public:
    MapCanvas( QWidget *parent = 0 );
    ~MapCanvas();
    

    void ReadImg(const QString imgPath);
    void CloseCurrentImg();
    void choseOpenFileMethod(int toolChoice);
    void displayImage(QGraphicsPixmapItem* PixmapItem);
    

    
    QStandardItemModel* ImgMetaModel(){ return imgMetaModel; };
   
    void SetMetaModel( QStandardItemModel* model ){ this->imgMetaModel = model;};
    
    QStandardItemModel* FileListModel(){ return fileListModel; };
    
    FileListDelegate* getFileListDelegate(){ return fileListDelegate;};

    void SetFileListModel( QStandardItemModel* model ){ this->fileListModel = model; };

    QGraphicsScene* getScene(){ return myScene; };

    void SetmyScene(QGraphicsScene* Scene){ this->myScene = Scene; };


    ParentClassDialog* getdialog(){ return dialog; };

    void setDialog(ParentClassDialog* setdialog){ this->dialog = setdialog; };

    int* getImageIndex() {return imageIndex;};
    
    void setImageIndex(int* imagenowindex) { this->imageIndex = imagenowindex; };

    int *getImageTotalNumber() { return imageTotalNumber; };

    void setImageTotalNumber(int *imageTotalNumber){ this->imageTotalNumber = imageTotalNumber; };

    ProcessImage* getProcessImageMumber() { return processImageMumber;};
    
    QSize sizeHint() const;
signals:
    void finishProcessImage();

    void sendXYpoint(QString xyPoint);

public slots:
    void displayImageSolt(QGraphicsPixmapItem* PixmapItem);
   
    void startBlockSelection();

    void switchFatherLayer(int fatherLayer, const QString fatherLayerPath);

    void deleteImageLayer(int imageLayer, const QString fatherLayerPath);
    
    void ZoomIn() { ScaleImg( 1.1 ); };
    
    void ZoomOut(){ ScaleImg( 0.9 ); };
    
protected:
    void wheelEvent( QWheelEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    
private:
    void ShowBand( GDALRasterBand* band);
    void ShowImg( QList<GDALRasterBand*> *imgBand);
    void ShowImgInfor( const QString filename );
    void switchImgInfor(const QString filename);
    void ShowFileList( const QString filename );
    unsigned char* ImgSketch( float* buffer , GDALRasterBand* currentBand, int size, double noValue );
    
    void ScaleImg( double factor );
    
    QStandardItemModel *imgMetaModel;
    
    GDALDataset *poDataset;
   
    QStandardItemModel *fileListModel;
    
    FileListDelegate* fileListDelegate;

    float m_scaleFactor;
    
    bool m_showColor;

    bool BlockSelectionFlag;

    bool maskFlag;
    
    QList<GDALRasterBand*> bandList;
    
    QList<GDALRasterBand*> myBand;

    QPoint lastEventCursorPos;

    QGraphicsScene* myScene;

     
    ParentClassDialog* dialog;

    int* imageIndex;
  
    int* imageTotalNumber;

    ProcessImage* processImageMumber;

    QGraphicsRectItem* maskItem;

    QPointF startPoint;

    LinkedList *imagelinkedList;
};


#endif // MAPCANVAS_H
