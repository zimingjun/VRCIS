#include "UIMain.h"

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include "MapCanvas.h"
#include <QHBoxLayout>
#include <QThread>
#include <QTimer>
#include <opencv2/opencv.hpp>



UIImgTest::UIImgTest(QWidget *parent)
    : QMainWindow( parent )
{
    ui.setupUi( this );
    this->showMaximized();
    
    myMap = new MapCanvas( this );
    myMap->setMinimumSize( 1024, 768 );
    this->setCentralWidget( myMap );
    
    ui.tableView->setModel( myMap->ImgMetaModel() );
    ui.tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );


    ui.fileTreeView->setModel( myMap->FileListModel() );
    ui.fileTreeView->setItemDelegate(myMap->getFileListDelegate());
    ui.fileTreeView->setEditTriggers( QAbstractItemView::NoEditTriggers );
   
    QStandardItemModel *model = new QStandardItemModel;
    model->setColumnCount(1);
    QStandardItem* rootItem1 = new QStandardItem(QIcon(":/GUI/imageSources/tool.png"), "Basic tools");
    QStandardItem* childItem1 = new QStandardItem(QIcon(":/GUI/imageSources/mosaic.png"), "mosaic");
    QStandardItem* childItem2 = new QStandardItem(QIcon(":/GUI/imageSources/fusion.png"), "fusion");
    QStandardItem* childItem3 = new QStandardItem(QIcon(":/GUI/imageSources/changedetect.png"), "changeDetect");
    rootItem1->setChild(0, childItem1);
    rootItem1->setChild(1, childItem2);
    rootItem1->setChild(2, childItem3);
    QStandardItem* rootItem2 = new QStandardItem(QIcon(":/GUI/imageSources/deepLearning.png"), "Deep learning");
    QStandardItem* childItem4 = new QStandardItem(QIcon(":/GUI/imageSources/sarToOptical.png"), "SAR to optical image");
    QStandardItem* childItem5 = new QStandardItem(QIcon(":/GUI/imageSources/segment.png"), "SAM model");
    rootItem2->setChild(0, childItem4);
    rootItem2->setChild(1, childItem5);
    QStandardItem* rootItem3 = new QStandardItem(QIcon(":/GUI/imageSources/preprocessing.png"), "Preprocessing");
    QStandardItem* childItem6 = new QStandardItem(QIcon(":/GUI/imageSources/sar.png"), "SAR preprocessing");
    rootItem3->setChild(0, childItem6);
    QStandardItem* segmentItem = new QStandardItem(QIcon(":/GUI/imageSources/segmentiation.png"), "segment");
    QStandardItem* segmentchild1 = new QStandardItem(QIcon(":/GUI/imageSources/Mrf.png"), "MRF");
    QStandardItem* segmentchild2= new QStandardItem(QIcon(":/GUI/imageSources/cluster.png"), "Cluster");
    segmentItem->setChild(0, segmentchild1);
    segmentItem->setChild(1, segmentchild2);
    model->appendRow(rootItem1);
    model->appendRow(rootItem2);
    model->appendRow(rootItem3);
    model->appendRow(segmentItem);
    ui.toolTreeView->setModel(model);
    toolBoxdelegate = new ToolBoxItemDelegate(ui.toolTreeView);
    ui.toolTreeView->setItemDelegate(toolBoxdelegate);
    ui.toolTreeView->expandAll();
    

    ui.statusBar->setSizeGripEnabled(false);

    ui.statusBar->showMessage(tr("welcome VRCIS!"),2000);
    QLabel* permanent = new QLabel(this);
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanent->setText(tr("<a href=\"https://github.com/zimingjun/VRCIS\">ABOUT VRCIS</a>"));
    permanent->setOpenExternalLinks(true);
    
    
    QProgressBar* pProgressBar = new QProgressBar();
    //pProgressBar->setStyleSheet("QProgressBar{height:22px; text-align:center; font-size:14px; color:white; border-radius:11px; background:#1D5573;}"
    //"QProgressBar::chunk{border-radius:11px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #99ffff,stop:1 #9900ff);}");
    QPushButton *taskbutton = new QPushButton("Task Manager", this);
    connect(taskbutton, &QPushButton::clicked, [&]() {
        MessageDialog *asasasa=new MessageDialog(this);
        asasasa->exec();
        });
    

    QLabel* pLabel = new QLabel();
    pLabel->setText("load image...");
    pProgressBar->setRange(0, 100);
    ui.statusBar->addPermanentWidget(pLabel);
    ui.statusBar->addPermanentWidget(pProgressBar);
    ui.statusBar->addPermanentWidget(permanent);
    ui.statusBar->addPermanentWidget(taskbutton);
    pLabel->setVisible(false);
    pProgressBar->setVisible(false);


    myMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    
    
    // 连接信号、槽
    //主界面类
    connect( ui.actionImage, SIGNAL( triggered() ), this, SLOT( PickOpenFile() ) );
    connect( ui.actionExit, SIGNAL( triggered() ), this, SLOT( Exit() ) );
    connect( ui.actionFile_List, SIGNAL( triggered() ), this, SLOT( ShowFileListWindow() ) );
    connect( ui.actionInformation, SIGNAL( triggered() ), this, SLOT( ShowInforWindow() ) );
    connect( ui.actionZoom_In, SIGNAL( triggered() ), this->myMap, SLOT( ZoomIn() ) );
    connect( ui.actionZoom_Out, SIGNAL( triggered() ), this->myMap, SLOT( ZoomOut() ) );
    connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(CloseImg()));
    connect(ui.actionblockSelection, SIGNAL(triggered()), this->myMap, SLOT(startBlockSelection()));
    connect(this->myMap, &MapCanvas::sendXYpoint, this, [=](QString xyPoint)
        {ui.statusBar->showMessage(xyPoint); });
    //工具功能类
    connect(toolBoxdelegate, &ToolBoxItemDelegate::mosaicToolButton, this, &UIImgTest::openMosaicWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::fusionToolButton, this, &UIImgTest::openFusionWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::changeDetectToolButton, this, &UIImgTest::openChangedetectWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::segmentMRFToolButton, this, &UIImgTest::openSegmentMRFWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::segmentClusterToolButton, this, &UIImgTest::openSegmentClusterWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::preprocessingToolButton, this, &UIImgTest::openPreprocessingWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::samToolButton, this, &UIImgTest::samWindows);
    connect(toolBoxdelegate, &ToolBoxItemDelegate::sar2opticalToolButton, this, &UIImgTest::sar2opticalWindows);
    
    
    //自定义类
    FileListDelegate* delegate;
    delegate = myMap->getFileListDelegate();
    connect(delegate, &FileListDelegate::sendFatherLayerNumber, this, &UIImgTest::handleFatherLayerNumber);
    connect(delegate, &FileListDelegate::sendDeleteLayerNumber, this, &UIImgTest::handlDeleteLayerNumber);

    ParentClassDialog* dialog;
    dialog = myMap->getdialog();
    connect(dialog, &ParentClassDialog::sendImageMosaicdata, this, &UIImgTest::handleImageMosaicdata);
    connect(dialog, &ParentClassDialog::sendImageFusiondata, this, &UIImgTest::handleImageFusiondata);
    connect(dialog, &ParentClassDialog::sendImageChangeDetectdata, this, &UIImgTest::handleImageChangeDetectdata);
    connect(dialog, &ParentClassDialog::sendImageSegmentdata, this, &UIImgTest::handleImageSegmentdata);

    Preprocessing = new PreprocessingDialog(this);
    connect(Preprocessing, &PreprocessingDialog::sendImagePreprocessingdata, this, &UIImgTest::handleImagePreprocessingDialogdata);

    segmentdialog = new SegmentDialog(this);
    //connect(Preprocessing, &PreprocessingDialog::sendImagePreprocessingdata, this, &UIImgTest::handleImagePreprocessingDialogdata);


    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
   connect(processImageMumber, &ProcessImage::sendProcessBarNumber, this, [=](int number) {
       QPropertyAnimation* pAnimation = new QPropertyAnimation(pProgressBar, "value");
       if (number == 100) {
           
           pProgressBar->setValue(100);
           ui.statusBar->showMessage(tr("finish process image!"), 2000);
           
           return;
       }
       pAnimation->setDuration(1000);
       pAnimation->setStartValue(pProgressBar->value());
       pAnimation->setEndValue(number);
       pProgressBar->setValue(number);
       QEventLoop loop;
       connect(pAnimation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
       pAnimation->start();
       loop.exec();
        });
    connect(processImageMumber, &ProcessImage::startProcessImage, this, [=]() {
        
        pLabel->setVisible(true);
        pProgressBar->setVisible(true);
        pProgressBar->setValue(0);
        });
    connect(processImageMumber, &ProcessImage::finishProcessImage, this, [=]() {
        pProgressBar->setValue(0);
        pLabel->setVisible(false);
        pProgressBar->setVisible(false);
        //delete pAnimation;
        });
}

UIImgTest::~UIImgTest()
{

}

void UIImgTest::Exit()
{
    this->close();
}

void UIImgTest::startTask()
{
    QMessageBox::information(this, "Task Progress", "Task started!");

    // Simulating task progress
    for (int i = 0; i <= 100; ++i) {
        //updateProgress(i);
        QCoreApplication::processEvents();
        QThread::msleep(50);
    }
}
void UIImgTest::PickOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                           this,
                           tr( "Pick a image file to open..." ),
                           QDir::currentPath(),
                           tr( "tiff(*.tif);;jpg(*.jpg);;img(*.img);;All files(*.*)" ) );
    if ( !fileName.isNull() )
    {
        
        myMap->ReadImg( fileName );
    }
    ui.fileTreeView->expandAll();
}

void UIImgTest::ShowFileListWindow()
{
    ui.FileListDockWidget->toggleViewAction();
}

void UIImgTest::ShowInforWindow()
{
    ui.inforDockWidget->toggleViewAction();
}

void UIImgTest::CloseImg()
{
    myMap->CloseCurrentImg();
}

void UIImgTest::openMosaicWindows()
{
    
    myMap->choseOpenFileMethod(1);

}

void UIImgTest::openFusionWindows()
{
    myMap->choseOpenFileMethod(2);
}

void UIImgTest::openChangedetectWindows()
{
    myMap->choseOpenFileMethod(3);
}

void UIImgTest::openPreprocessingWindows()
{
    
    Preprocessing->exec();
}

void UIImgTest::samWindows()
{
	/*system("start D:/myproject/VRCIS_main/x64/Debug/SAM.exe - pre_model = \"D:/myproject/segment-anything-cpp-wrapper-main/build/Debug/models/sam_preprocess.onnx\" "
		"-sam_model=\"D:/myproject/segment-anything-cpp-wrapper-main/build/Debug/models/sam_vit_h_4b8939.onnx\" "
		"-image=\"D:/project/testimage/123.jpg\" -pre_device=\"cpu\" -sam_device=\"cuda:0\"");*/

    /*segmentstart a;
    a.ProcessImage();*/
    
}

void UIImgTest::sar2opticalWindows()
{
    torch::DeviceType device_type;
    if (torch::cuda::is_available()) {
        std::cout << "CUDA available! Predicting on GPU." << std::endl;
        device_type = torch::kCUDA;
    }
    else {
        std::cout << "Predicting on CPU." << std::endl;
        device_type = torch::kCPU;
    }
    torch::Device device(device_type);

    //Init model
    std::string model_pb = "C://Users//MSI//Desktop//python//xiangmu//xiangmu//damn2.pt";
    auto module = torch::jit::load(model_pb);
    module.to(at::kCUDA);


    torch::Tensor tensor_image = torch::empty({ 1, 256, 256, 3 }, torch::kFloat32);

    for (int i = 1; i < 2; i++) {
        std::string image_path = "C://Users//MSI//Desktop//python//xiangmu//xiangmu//results//combine_whu_hcgan0906//test_latest//images//beijing_" + std::to_string(i) + "_real_A.png";
        cv::Mat image = cv::imread(image_path, cv::ImreadModes::IMREAD_COLOR);

        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        image.convertTo(image, CV_32FC3, 1.0f / 255.0f);
        image = (image - 0.5) / 0.5;

        // 拷贝图像数据到张量中
        float* data_ptr = tensor_image[i - 1].data_ptr<float>();
        std::memcpy(data_ptr, image.data, 256 * 256 * 3 * sizeof(float));
    }


    //auto image = cv::imread("G://damn.png", cv::ImreadModes::IMREAD_COLOR);
    //cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    ////cv::resize(image, image, cv::Size(256, 256));
    //image.convertTo(image, CV_32FC3, 1.0f / 255.0f);  
    //image = (image - 0.5) / 0.5;  

    /*torch::Tensor tensor_image = torch::from_blob(image.data, { 1, 256, 256, 3 }, torch::kFloat32);*/
    tensor_image = tensor_image.permute({ 0, 3, 1, 2 });
    tensor_image = tensor_image.to(torch::kCUDA);

    torch::Tensor output = module.forward({ tensor_image }).toTensor();
    output = output.select(0, 0);
    output = output.to(torch::kCPU);
    std::cout << "Tensor shape: " << tensor_image.sizes() << std::endl;
    std::cout << "Tensor shape: " << output.sizes() << std::endl;
    output = (output + 1.0) * 127.5;
    ////cv::Mat output_image(256, 256, CV_32FC3, output.data_ptr<float>());
    ////cv::imshow("Output Image", output_image);
    cv::Mat image3(256, 256, CV_32FC3);
    float* data_ptr = output.data_ptr<float>();
    for (int c = 2, b = 0; c >= 0; c--, b++) {
        for (int h = 0; h < 256; h++) {
            for (int w = 0; w < 256; w++) {
                image3.at<cv::Vec3f>(h, w)[c] = (int)(data_ptr[b * 256 * 256 + h * 256 + w]);
            }
        }
    }
    cv::imwrite("G://ok_damn.png", image3);
    //auto image33 = cv::imread("G://damn.png", cv::ImreadModes::IMREAD_COLOR);
    auto image44 = cv::imread("G://ok_damn.png", cv::ImreadModes::IMREAD_COLOR);
    //cv::imshow("damn", image33);
    cv::imshow("ok_damn", image44);
    cv::waitKey(0);
}

void UIImgTest::openSegmentMRFWindows()
{
    myMap->choseOpenFileMethod(4);
}

void UIImgTest::openSegmentClusterWindows()
{
    myMap->choseOpenFileMethod(5);
}


void UIImgTest::handleFatherLayerNumber(int itemNumber , const QString fatherLayerPath) {
    
    myMap->switchFatherLayer(itemNumber, fatherLayerPath);
    
}
void UIImgTest::handlDeleteLayerNumber(int itemNumber, const QString fatherLayerPath)
{
    myMap->deleteImageLayer(itemNumber, fatherLayerPath);
}


void UIImgTest::handleImageFusiondata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod)
{
    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
    fusionthread = new PcaFusion(ImageFusionPath1.toStdString().c_str(), ImageFusionPath2.toStdString().c_str(), ImageFusionSavePath.toStdString().c_str(), ImageFusionMethod.toStdString().c_str());
    connect(fusionthread, &PcaFusion::finishFusion, this, &UIImgTest::finishImageFusionSlot);
    connect(fusionthread, &PcaFusion::sendStatueBarMessage, this, &UIImgTest::receiveStatueBarMessage);
    connect(fusionthread, &PcaFusion::sendProcessBarNumber, processImageMumber, &ProcessImage::receiveProcessBarNumber);
    connect(fusionthread, &PcaFusion::startFusionImage, processImageMumber, &ProcessImage::startProcessImageSlot);
    connect(fusionthread, &PcaFusion::finishFusionImage, processImageMumber, &ProcessImage::finishProcessImageSlot);

    fusionthread->start();

}

void UIImgTest::handleImageChangeDetectdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod)
{
    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
    cdthread = new CD(ImageFusionPath1.toStdString().c_str(), ImageFusionPath2.toStdString().c_str(), ImageFusionSavePath.toStdString().c_str(), ImageFusionMethod.toStdString().c_str());
    connect(cdthread, &CD::finishChangeDetect, this, &UIImgTest::finishImageChangeDetectSlot);
    connect(cdthread, &CD::sendStatueBarMessage, this, &UIImgTest::receiveStatueBarMessage);
    connect(cdthread, &CD::sendProcessBarNumber, processImageMumber, &ProcessImage::receiveProcessBarNumber);
    connect(cdthread, &CD::startChangeDetectImage, processImageMumber, &ProcessImage::startProcessImageSlot);
    connect(cdthread, &CD::finishChangeDetectImage, processImageMumber, &ProcessImage::finishProcessImageSlot);

    cdthread->start();
}

void UIImgTest::handleImageSegmentdata(const QString ImageFusionPath1, const QString ImageFusionSavePath, const QString ImageFusionMethod)
{
    segmentdialog->exec();
    std::vector<int> intParamater; std::vector<double> doubleParamater; std::vector<std::string> stringParamater;
    intParamater.push_back(segmentdialog->getui().nClusterCnt->value());
    intParamater.push_back(segmentdialog->getui().dbBeta1->value());
    intParamater.push_back(segmentdialog->getui().nInit1->value());
    intParamater.push_back(segmentdialog->getui().nInit2->value());
    intParamater.push_back(segmentdialog->getui().nInitIterFinal->value());
    intParamater.push_back(segmentdialog->getui().nOuterIter->value());
    doubleParamater.push_back(segmentdialog->getui().dbBeta2->value());
    doubleParamater.push_back(segmentdialog->getui().dbSAInitT->value());
    stringParamater.push_back(ImageFusionMethod.toStdString());
    stringParamater.push_back(segmentdialog->getui().pInitMethod->currentText().toStdString());
    stringParamater.push_back(segmentdialog->getui().pInitInitMethod->currentText().toStdString());
    stringParamater.push_back(ImageFusionPath1.toStdString());
    stringParamater.push_back(ImageFusionSavePath.toStdString());
    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
    segmentThread = new segmentstart(intParamater, doubleParamater, stringParamater);
    connect(segmentThread, &segmentstart::finishSegment, this, &UIImgTest::finishImageSegmentSlot);
    connect(segmentThread, &segmentstart::sendStatueBarMessage, this, &UIImgTest::receiveStatueBarMessage);
    connect(segmentThread, &segmentstart::sendProcessBarNumber, processImageMumber, &ProcessImage::receiveProcessBarNumber);
    connect(segmentThread, &segmentstart::startSegmentImage, processImageMumber, &ProcessImage::startProcessImageSlot);
    connect(segmentThread, &segmentstart::finishSegmentImage, processImageMumber, &ProcessImage::finishProcessImageSlot);

    segmentThread->start();
}

void UIImgTest::finishImageSegmentSlot(const QString ImageMasaicPath)
{
    delete segmentThread;
    myMap->ReadImg(ImageMasaicPath);
    ui.statusBar->showMessage(tr("finish Segment"), 2000);
}

void UIImgTest::handleImageMosaicdata(const QString ImageFusionPath1, const QString ImageFusionPath2, const QString ImageFusionSavePath, const QString ImageFusionMethod)
{
    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
    thread = new Mosaic(ImageFusionPath1.toStdString().c_str(), ImageFusionPath2.toStdString().c_str(), ImageFusionSavePath.toStdString().c_str(), ImageFusionMethod.toStdString().c_str());
    connect(thread, &Mosaic::finishmosaic, this, &UIImgTest::finishImageMosaicSlot);
    connect(thread, &Mosaic::sendStatueBarMessage, this, &UIImgTest::receiveStatueBarMessage);
    connect(thread, &Mosaic::sendProcessBarNumber, processImageMumber, &ProcessImage::receiveProcessBarNumber);
    connect(thread, &Mosaic::startMosaicImage, processImageMumber, &ProcessImage::startProcessImageSlot);
    connect(thread, &Mosaic::finishMosaicImage, processImageMumber, &ProcessImage::finishProcessImageSlot);

    thread->start();
    
}

void UIImgTest::finishImageMosaicSlot(const QString ImageMasaicPath)
{
    delete thread;
    myMap->ReadImg(ImageMasaicPath);
    ui.statusBar->showMessage(tr("finish Mosaic"), 2000);
}
void UIImgTest::finishImageChangeDetectSlot(const QString ImageMasaicPath)
{
    delete cdthread;
    myMap->ReadImg(ImageMasaicPath);
    ui.statusBar->showMessage(tr("finish ChangeDetect"), 2000);
}
void UIImgTest::handleImagePreprocessingDialogdata(const QString ImagePath, const QString ImagexmlPath, const QString ImageSavePath)
{
    ProcessImage* processImageMumber;
    processImageMumber = myMap->getProcessImageMumber();
    sarPreprocessingthread = new sarPreprocessing(ImagePath.toStdString().c_str(), ImagexmlPath.toStdString().c_str(), ImageSavePath.toStdString().c_str());
    connect(sarPreprocessingthread, &sarPreprocessing::finishPreprocessing, this, &UIImgTest::finishImagePreprocessingSlot);
    connect(sarPreprocessingthread, &sarPreprocessing::sendStatueBarMessage, this, &UIImgTest::receiveStatueBarMessage);
    connect(sarPreprocessingthread, &sarPreprocessing::sendProcessBarNumber, processImageMumber, &ProcessImage::receiveProcessBarNumber);
    connect(sarPreprocessingthread, &sarPreprocessing::startPreprocessingImage, processImageMumber, &ProcessImage::startProcessImageSlot);
    connect(sarPreprocessingthread, &sarPreprocessing::finishPreprocessingImage, processImageMumber, &ProcessImage::finishProcessImageSlot);

    sarPreprocessingthread->start();

}

void UIImgTest::finishImagePreprocessingSlot(const QString ImageMasaicPath)
{
    delete sarPreprocessingthread;
    myMap->ReadImg(ImageMasaicPath);
    ui.statusBar->showMessage(tr("finish sarPreprocessing"), 2000);
}

void UIImgTest::finishImageFusionSlot(const QString ImageMasaicPath)
{
    delete fusionthread;
    myMap->ReadImg(ImageMasaicPath);
    ui.statusBar->showMessage(tr("finish Fusion"), 2000);
}
void UIImgTest::receiveStatueBarMessage(QString Message)
{
    ui.statusBar->showMessage(Message);
}

void UIImgTest::receiveProcessBarNumber(int number)
{
    //std::cout << number;
    pProgressBar->setValue(number);
    
   

}

