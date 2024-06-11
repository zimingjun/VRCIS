#include "pick_opened_file.h"
ParentClassDialog::ParentClassDialog(QStandardItemModel* model, QWidget* parent )
    : QDialog(parent), m_model(model) {
    setWindowTitle("chose image file");
    resize(1000, 500);
    QVBoxLayout* layout = new QVBoxLayout(this);


    QStringList stringList = convertStandardItemModelToStringList(model);
    m_listModel = new QStringListModel(stringList, this);

    m_listView = new QListView(this);
    m_listView->setModel(m_listModel);
    m_listView->setSelectionMode(QAbstractItemView::MultiSelection);
    layout->addWidget(m_listView);


    QHBoxLayout* fileLayout = new QHBoxLayout;

    m_chooseButton = new QPushButton("chose save path", this);
    connect(m_chooseButton, &QPushButton::clicked, this, &ParentClassDialog::chooseSaveLocationClicked);

    m_fileLabel = new QLabel(this);
    m_fileLabel->setText("savePath:");

    fileLayout->addWidget(m_chooseButton);
    fileLayout->addWidget(m_fileLabel);

    layout->addLayout(fileLayout);

    comboLayout = new QHBoxLayout;

    m_combo = new QComboBox(this);


    layout->addLayout(comboLayout);


    m_selectButton = new QPushButton("OK", this);
    m_selectButton->setEnabled(false);
    connect(m_selectButton, &QPushButton::clicked, this, &ParentClassDialog::selectClicked);

    layout->addWidget(m_selectButton);
    InitializeSelection = true;
    segmentMethod = false;
}
void ParentClassDialog::chooseSaveLocationClicked() 
{
    QString saveLocation = QFileDialog::getSaveFileName(this, (segmentMethod == true) ? "chose mask save path": "chose save path" , QString(), "IMAGE FILE (*.tif *.jpg)");
    if (!saveLocation.isEmpty()) {
        m_fileLabel->setText(saveLocation);
        m_selectButton->setEnabled(true);
    }
}

void ParentClassDialog::selectClicked() 
{
    QModelIndexList selectedIndexes = m_listView->selectionModel()->selectedIndexes();

    if (selectedIndexes.size() != 2 && segmentMethod ==false ) {
        QMessageBox::warning(this, "error", "only can chose two pictures");
    }
    else if (selectedIndexes.size() != 1 && segmentMethod == true) {
        QMessageBox::warning(this, "error", "only can chose one pictures");
    }
    else {
        QString selectedText1 = selectedIndexes.at(0).data().toString();
        QString selectedText2;
        if(segmentMethod == false)
        {
            QString selectedText2 = selectedIndexes.at(1).data().toString();
        }

        QString comboText = m_combo->currentText();

        QString savepathText = m_fileLabel->text();

        if (m_fileLabel->text().isEmpty()) {
            QMessageBox::warning(this, "error", "please set savepath");
            return;
        }
        close();

        if (toolChoice == 1)
        {
            emit sendImageMosaicdata(selectedText1, selectedText2, savepathText, comboText);

        }
        if (toolChoice == 2)
        {

            emit sendImageFusiondata(selectedText1, selectedText2, savepathText, comboText);

        }
        if (toolChoice == 3)
        {

            emit sendImageChangeDetectdata(selectedText1, selectedText2, savepathText, comboText);

        }
        if (toolChoice == 4)
        {

            emit sendImageSegmentdata(selectedText1, savepathText, comboText);

        }
        if (toolChoice == 5)
        {

            emit sendImageSegmentdata(selectedText1, savepathText, comboText);

        }


    }
}
void ParentClassDialog::setFileListModel(QStandardItemModel* model) {
    m_model = model;
    QStringList stringList = convertStandardItemModelToStringList(model);
    m_listModel->setStringList(stringList);
}
void ParentClassDialog::setToolChoice(int toolUserChoice)
{
    InitializeSelection = false;
    if (!InitializeSelection)
    {
        m_combo->clear();
        comboLayout->removeWidget(m_combo);
    }
    if (toolUserChoice == 1) {
        toolChoice = imageMosaic;
    }
    if (toolUserChoice == 2) {
        toolChoice = imageFusion;
    }
    if (toolUserChoice == 3) {
        toolChoice = imageChangeDetect;
    }
    if (toolUserChoice == 4) {
        toolChoice = imageMRFSegment;
    }
    if (toolUserChoice == 5) {
        toolChoice = imageClusterSegment;
    }
    if (toolChoice == 1)
    {
        m_combo->addItem("CPU");
        m_combo->addItem("GPU");

    }
    if (toolChoice == 2)
    {
        m_combo->addItem("PCA");
        m_combo->addItem("HPF");
        m_combo->addItem("Multi1");
        m_combo->addItem("Multi2");
        m_combo->addItem("Brovey");
        m_combo->addItem("Correlation");

    }
    if (toolChoice == 3)
    {
        m_combo->addItem("sar algorithm 1");
        m_combo->addItem("sar algorithm 2");
        m_combo->addItem("optical algorithm");

    }
    if (toolChoice == 4)
    {
        m_combo->addItem("IRGS");
        m_combo->addItem("CMLL");
        m_combo->addItem("VMLL");
        m_combo->addItem("GIEP");
        m_combo->addItem("Polarimetric IRGS");
        segmentMethod = true;

    }
    if (toolChoice == 5)
    {
        m_combo->addItem("K - means");
        m_combo->addItem("Gaussian Mixture Model");
        m_combo->addItem("Otus");
        segmentMethod = true;

    }

    comboLayout->addWidget(m_combo);
}
QStringList ParentClassDialog::convertStandardItemModelToStringList(QStandardItemModel* model) 
{
    QStringList stringList;
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0);
        //QString itemText = index.data().toString();
        //stringList.append(itemText);
        QVariant tooltipData = index.data(Qt::ToolTipRole);
        const QString tooltip = tooltipData.toString();
        stringList.append(tooltip);
    }
    return stringList;
}