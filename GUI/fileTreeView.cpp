#include "fileTreeView.h"

 
    

void FileListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        painter->save();
        painter->setPen(Qt::gray);

        QString text = index.data(Qt::DisplayRole).toString();

        if (option.state & QStyle::State_MouseOver) {
            QFont boldFont = QApplication::font();
            boldFont.setBold(true);
            painter->setFont(boldFont);
        }

        QRect textRect = option.rect.adjusted(0, 0, -20, 0);

        painter->drawText(textRect, opt.displayAlignment, text);

        painter->restore();
    }

bool FileListDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
    {
        if (event->type() == QEvent::MouseButtonDblClick && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton)
        {
            QModelIndex parentIndex = index.parent();
            if (parentIndex.isValid()) {
                int parentNumber = parentIndex.row() + 1;
                int childNumber = index.sibling(index.row(), 0).row() + 1;

                qDebug() << "Double clicked on child item with parent index:" << parentNumber << "and child number:" << childNumber;
                // 子类业务逻辑处理
                // 功能待加入

            }
            else {
                int itemNumber = index.row() + 1;
                QVariant tooltipData = index.data(Qt::ToolTipRole);
                const QString tooltip = tooltipData.toString();

                emit sendFatherLayerNumber(itemNumber, tooltip);

            }

            return true;
        }
        if (event->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent*>(event)->button() == Qt::RightButton)
        {
            QModelIndex parentIndex = index.parent();
            if (parentIndex.isValid()) {


            }
            else {

                QMenu menu;

                QAction* deleteAction = menu.addAction("Delete");

                fatherItemNumber = index.row() + 1;

                QVariant tooltipData = index.data(Qt::ToolTipRole);

                fatherLayerPath = tooltipData.toString();

                connect(deleteAction, &QAction::triggered, this, &FileListDelegate::onDeleteTriggered);

                menu.exec(QCursor::pos());


            }
            return true;
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }