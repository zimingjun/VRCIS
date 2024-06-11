#include "MapCanvas.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>
#include "UIMain.h"
#include <iostream>
#include <qDebug>


MapCanvas::MapCanvas( QWidget *parent /*= 0 */ )
    : QGraphicsView( parent )
{
    poDataset = NULL;
    m_scaleFactor = 1.0;
    m_showColor = true;
    imgMetaModel = new QStandardItemModel;
    fileListDelegate = new FileListDelegate;
    imgMetaModel->setColumnCount( 2 );
    fileListModel = new QStandardItemModel;
    QSizePolicy policy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    this->setSizePolicy( policy );
    myScene = new QGraphicsScene();
    dialog = new ParentClassDialog(fileListModel);
    imageIndex =new int(0);
    imageTotalNumber = new int(0);

    processImageMumber = new ProcessImage();
    connect(processImageMumber, &ProcessImage::sendProcessedImage, this, &MapCanvas::displayImageSolt);
    
    setMouseTracking(true);

    maskItem = nullptr;

    BlockSelectionFlag = false;

    maskFlag = false;

    imagelinkedList = new LinkedList(15);
}

MapCanvas::~MapCanvas()
{
    /*// 遍历场景中的所有图形项
    QList<QGraphicsItem*> items = myScene->items();
    for (QGraphicsItem* item : items) 
    {
        // 从场景中移除图形项
        myScene->removeItem(item);
        // 删除图形项对象
        delete item;
    }
    CloseCurrentImg();*/
    //delete imgItem;
    //delete myScene;
}



void MapCanvas::CloseCurrentImg()
{
    int imageindexnow = *getImageIndex();
    myScene->removeItem(imagelinkedList->getNodeData(*imageIndex - 1));
    imagelinkedList->clear();
    *imageIndex = 0;
    *imageTotalNumber = 0;
    poDataset = NULL;
    imgMetaModel->clear();
    fileListModel->clear();
}

void MapCanvas::choseOpenFileMethod(int toolChoice)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("chose file method");
    msgBox.setText("opened or pick new file?");
    QPushButton* yesButton = msgBox.addButton("opened", QMessageBox::YesRole);
    QPushButton* noButton = msgBox.addButton("pick new file", QMessageBox::NoRole);
    QPushButton* cancelButton = msgBox.addButton("Cancel", QMessageBox::RejectRole);
    msgBox.setDefaultButton(noButton);
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {

        dialog->setFileListModel(fileListModel);
        dialog->setToolChoice(toolChoice);
        dialog->exec();
    }
    else if (msgBox.clickedButton() == noButton) {
       
    }
    else if (msgBox.clickedButton() == cancelButton) {

    }


}




void MapCanvas::startBlockSelection()
{
    if (BlockSelectionFlag)BlockSelectionFlag = false;
    else BlockSelectionFlag = true;
}

void MapCanvas::switchFatherLayer(int fatherLayer, const QString fatherLayerPath)
{
    
    if(*imageIndex!=fatherLayer)
    {
        //myScene->removeItem(&imgItem[*imageIndex]);
        myScene->removeItem(imagelinkedList->getNodeData(*imageIndex-1));
        //displayImage(&imgItem[fatherLayer]);
        displayImage(imagelinkedList->getNodeData(fatherLayer - 1));
        imgMetaModel->clear();
        switchImgInfor(fatherLayerPath);
        *imageIndex = fatherLayer;
    }
    
}
void MapCanvas::deleteImageLayer(int imageLayer, const QString fatherLayerPath)
{
    
    if (*imageIndex != imageLayer)
    {

        imagelinkedList->remove(imagelinkedList->getNodeNumber(imageLayer - 1));
        *imageIndex = imageLayer;
        fileListModel->removeRow(imageLayer-1);
        (*imageTotalNumber)--;
    }
    else
    {
        myScene->removeItem(imagelinkedList->getNodeData(*imageIndex - 1));
        if (*imageIndex == 1)
        {
            imgMetaModel->clear();
            fileListModel->removeRow(0);
            *imageIndex = 0;
            imgMetaModel->clear();
            *imageTotalNumber = 0;
            imagelinkedList->remove(imagelinkedList->getNodeNumber(imageLayer - 1));
        }
        else
        {
            displayImage(imagelinkedList->getNodeData(imageLayer - 2));
            imgMetaModel->clear();
            switchImgInfor(fatherLayerPath);
            imagelinkedList->remove(imagelinkedList->getNodeNumber(imageLayer - 1));
            (*imageIndex)--;
            fileListModel->removeRow(imageLayer-1);
            (*imageTotalNumber)--;
        }
        
    }
}
void MapCanvas::ReadImg(const QString imgPath)
{
    int itemCount = fileListModel->rowCount();
    if (itemCount != 0)
    {
        qDeleteAll(myBand.begin(), myBand.end());
        myBand.clear();
        qDeleteAll(bandList.begin(), bandList.end());
        bandList.clear();
        imgMetaModel->clear();
        myScene->removeItem(imagelinkedList->getNodeData(*imageIndex - 1));
    }
    
    //最大支持32000*32000图片大小
    //优化思路：如果大于32000*32000则，切片为4片
    //处理时占用内存较大，大约为图片大小的3倍，以空间换速率
    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
    processImageMumber->setProcessImage(imgPath);
    processImageMumber->start();


    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
    poDataset = (GDALDataset*)GDALOpen(imgPath.toStdString().c_str(), GA_ReadOnly);
    ShowFileList(imgPath);
    ShowImgInfor(imgPath);
    GDALClose(poDataset);
    
}
void MapCanvas::ShowBand( GDALRasterBand* band )
{
    if ( band == NULL )
    {
        return;
    }
    
    QList<GDALRasterBand*> myBand;
    myBand.append( band );
    myBand.append( band );
    myBand.append( band );
    
    ShowImg( &myBand );
    
}

void MapCanvas::ShowImg( QList<GDALRasterBand*> *imgBand )
{
    
   

}
void MapCanvas::displayImage(QGraphicsPixmapItem* PixmapItem)
{
    
    myScene->addItem(PixmapItem);
    this->setScene(myScene);
    this->setSceneRect(myScene->itemsBoundingRect());
    QRectF sceneRect = myScene->itemsBoundingRect();
    sceneRect.setWidth(sceneRect.width() * this->height() / sceneRect.height());
    sceneRect.setHeight(this->height());
    this->fitInView(sceneRect, Qt::KeepAspectRatio);
    this->viewport()->update();
    //this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void MapCanvas::displayImageSolt(QGraphicsPixmapItem* PixmapItem)
{
    (*imageTotalNumber)++;
    *imageIndex = *imageTotalNumber;
    
    //QPixmap pixmap = PixmapItem->pixmap();
    imagelinkedList->append(PixmapItem);

    //imgItem[*imageTotalNumber].setPixmap(PixmapItem->pixmap());

    displayImage(imagelinkedList->getNodeData(*imageTotalNumber-1));
    //displayImage(&imgItem[*imageTotalNumber]);
    //delete PixmapItem;
    //emit finishProcessImage();
}

void MapCanvas::ShowImgInfor( const QString filename )
{
    if ( filename == "" || poDataset == NULL )
    {
        return;
    }



    int row = 0;
    
    
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Description" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( poDataset->GetDriver()->GetDescription() ) );
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Meta Infor" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) ) ) ;
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Data Type" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( GDALGetDataTypeName( ( poDataset->GetRasterBand( 1 )->GetRasterDataType() ) ) ) );
    
    
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "X Size" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( QString::number( poDataset->GetRasterXSize() ) ) );
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Y Size" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( QString::number( poDataset->GetRasterYSize() ) ) );
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Band Count" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( QString::number( poDataset->GetRasterCount() ) ) );
    
    
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Projection" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( poDataset->GetProjectionRef() ) );
    
   
    double adfGeoTransform[6];
    QString origin = "";
    QString pixelSize = "";
    if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        origin = QString::number( adfGeoTransform[0] ) + ", " + QString::number( adfGeoTransform[3] );
        pixelSize = QString::number( adfGeoTransform[1] ) + ", " + QString::number( adfGeoTransform[5] );
    }
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Origin" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( origin ) );
    imgMetaModel->setItem( row, 0, new QStandardItem( tr( "Pixel Size" ) ) );
    imgMetaModel->setItem( row++, 1, new QStandardItem( pixelSize ) );
}
void MapCanvas::switchImgInfor(const QString filename)
{
    if (filename == "" || poDataset == NULL)
    {
        return;
    }


    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

    poDataset = (GDALDataset*)GDALOpen(filename.toStdString().c_str(), GA_ReadOnly);

    int row = 0;


    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Description")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(poDataset->GetDriver()->GetDescription()));
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Meta Infor")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(poDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME)));
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Data Type")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(GDALGetDataTypeName((poDataset->GetRasterBand(1)->GetRasterDataType()))));


    imgMetaModel->setItem(row, 0, new QStandardItem(tr("X Size")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(QString::number(poDataset->GetRasterXSize())));
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Y Size")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(QString::number(poDataset->GetRasterYSize())));
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Band Count")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(QString::number(poDataset->GetRasterCount())));


    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Projection")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(poDataset->GetProjectionRef()));


    double adfGeoTransform[6];
    QString origin = "";
    QString pixelSize = "";
    if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
    {
        origin = QString::number(adfGeoTransform[0]) + ", " + QString::number(adfGeoTransform[3]);
        pixelSize = QString::number(adfGeoTransform[1]) + ", " + QString::number(adfGeoTransform[5]);
    }
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Origin")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(origin));
    imgMetaModel->setItem(row, 0, new QStandardItem(tr("Pixel Size")));
    imgMetaModel->setItem(row++, 1, new QStandardItem(pixelSize));
    GDALClose(poDataset);
}

void MapCanvas::ShowFileList( const QString filepath )
{
    
    if (filepath == "" || poDataset == NULL)
    {
        return;
    }
    QFileInfo fileInfo(filepath);
    QStandardItem* rootItem = new QStandardItem(fileInfo.fileName()); 
    rootItem->setData(filepath, Qt::ToolTipRole); 
    for (int i = 0; i < poDataset->GetRasterCount(); i++) {
        QStandardItem* childItem = new QStandardItem(tr("Band %1").arg(i + 1));
        childItem->setData(filepath, Qt::ToolTipRole); 
        rootItem->setChild(i, childItem);
    }
    fileListModel->appendRow(rootItem);
   
}

unsigned char* MapCanvas::ImgSketch( float* buffer , GDALRasterBand* currentBand, int bandSize, double noValue )
{
    unsigned char* resBuffer = new unsigned char[bandSize];
    double max, min;
    double minmax[2];
    
    
    currentBand->ComputeRasterMinMax( 1, minmax );
    min = minmax[0];
    max = minmax[1];
    if( min <= noValue && noValue <= max )
    {
        min = 0;
    }
    for ( int i = 0; i < bandSize; i++ )
    {
        if ( buffer[i] > max )
        {
            resBuffer[i] = 255;
        }
        else if ( buffer[i] <= max && buffer[i] >= min )
        {
            resBuffer[i] = static_cast<uchar>( 255 - 255 * ( max - buffer[i] ) / ( max - min ) );
        }
        else
        {
            resBuffer[i] = 0;
        }
    }
    
    return resBuffer;
}

void MapCanvas::ScaleImg(double factor)
{
    m_scaleFactor *= factor;
    QMatrix matrix;
    matrix.scale(m_scaleFactor, m_scaleFactor);
    this->setMatrix(matrix);
}

QSize MapCanvas::sizeHint() const
{
    return QSize( 1024, 768 );
}

void MapCanvas::wheelEvent( QWheelEvent *event )
{
    
    if ( event->delta() > 0 )
    {
        ZoomIn();
    }
    
    if ( event->delta() < 0 )
    {
        ZoomOut();
    }
}

void MapCanvas::mousePressEvent( QMouseEvent *event )
{
   
    if ( event->button() == Qt::LeftButton )
    {
        
        if (maskFlag)
        {

            myScene->removeItem(maskItem);
            delete maskItem;
            maskItem = nullptr;
        }
        if (BlockSelectionFlag) 
        {
            startPoint = mapToScene(event->pos());
            
            maskItem = new QGraphicsRectItem(startPoint.x(), startPoint.y(), 0, 0);
           
            maskItem->setBrush(QBrush(QColor(0, 255, 255, 60)));
            myScene->addItem(maskItem);
        }
        else
        {
            this->setDragMode(QGraphicsView::ScrollHandDrag);
            this->setInteractive(false);
            lastEventCursorPos = event->pos();
        }
        
    }
    QGraphicsView::mousePressEvent(event);
}

void MapCanvas::mouseMoveEvent( QMouseEvent *event )
{
    if(*imageTotalNumber!=0)
    {
        if (event->buttons() & Qt::LeftButton && maskItem)
        {
            QPointF currentPos = mapToScene(event->pos());
            qreal width = currentPos.x() - startPoint.x();
            qreal height = currentPos.y() - startPoint.y();
            maskItem->setRect(startPoint.x(), startPoint.y(), width, height);
        }
    QGraphicsView::mouseMoveEvent(event);
    QPointF mousePos = mapToScene(event->pos());
    QString posString = QString("(%1, %2)")
        .arg(mousePos.x())
        .arg(mousePos.y());
    emit sendXYpoint(posString);
    if ( this->dragMode() == QGraphicsView::ScrollHandDrag )
    {
        QPoint delta = ( event->pos() - lastEventCursorPos ) / 10;
        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() + ( isRightToLeft() ? delta.x() : -delta.x() ) );
        this->verticalScrollBar()->setValue( this->verticalScrollBar()->value() - delta.y() );
        this->viewport()->setCursor( Qt::ClosedHandCursor );
    }
    }
}

void MapCanvas::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton)
    {
        this->setDragMode( QGraphicsView::NoDrag );

        if (maskItem)
        {
            QPointF endPoint = mapToScene(event->pos());

            qreal width = endPoint.x() - startPoint.x();
            qreal height = endPoint.y() - startPoint.y();
            maskItem->setRect(startPoint.x(), startPoint.y(), width, height);
            maskFlag = true;
            //maskItem = nullptr; 
        }
        
    }
   
    QGraphicsView::mouseReleaseEvent(event);
}

