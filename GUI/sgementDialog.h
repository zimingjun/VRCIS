#pragma once
#include "ui_segmentDialog.h"
class SegmentDialog : public QDialog
{
    Q_OBJECT

public:
    SegmentDialog(QWidget* parent = 0);
    ~SegmentDialog() {}
    Ui::SegmentDialog getui() { return ui; }
private:
    Ui::SegmentDialog ui;
};