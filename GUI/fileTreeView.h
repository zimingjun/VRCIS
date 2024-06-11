#pragma once

#include <QStandardItemModel>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QFileInfo>
#include <QToolTip>
#include "mapcanvas.h"
#include "UIMain.h"

class FileListDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit FileListDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
    
private:

    int fatherItemNumber;

    QString fatherLayerPath;

signals:
    void sendFatherLayerNumber(int itemNumber, const QString fatherLayerPath);
    void sendDeleteLayerNumber(int itemNumber, const QString fatherLayerPath);
public slots:
    

    void onDeleteTriggered()
    {
        emit sendDeleteLayerNumber(fatherItemNumber, fatherLayerPath);
    }
};