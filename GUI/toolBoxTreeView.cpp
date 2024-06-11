#include "toolBoxTreeView.h"
bool ToolBoxItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        QModelIndex parentIndex = index.parent();
        if (parentIndex.isValid()) {
            int parentNumber = parentIndex.row() + 1;
            int childNumber = index.sibling(index.row(), 0).row() + 1;

            if (parentNumber == 1)
            {
                if (childNumber == 1)emit mosaicToolButton();
                if (childNumber == 2)emit fusionToolButton();
                if (childNumber == 3)emit changeDetectToolButton();
            }
            if (parentNumber == 2)
            {
                if (childNumber == 1)emit sar2opticalToolButton();
                if (childNumber == 2)emit samToolButton();

            }
            if (parentNumber == 3)
            {
                if (childNumber == 1)emit preprocessingToolButton();
                
            }
            if (parentNumber == 4)
            {
                if(childNumber == 1)emit segmentMRFToolButton();
                if (childNumber == 2)emit segmentClusterToolButton();

            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}