#pragma once
#include <QApplication>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QStyledItemDelegate>
#include <QMouseEvent>
#include <QDebug>
class ToolBoxItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ToolBoxItemDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        QStyledItemDelegate::setEditorData(editor, index);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        QStyledItemDelegate::setModelData(editor, model, index);
    }

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
signals:
    void mosaicToolButton();
    void changeDetectToolButton();
    void fusionToolButton(); 
    void preprocessingToolButton();
    void samToolButton();
    void segmentMRFToolButton();
    void segmentClusterToolButton();
    void sar2opticalToolButton();
};