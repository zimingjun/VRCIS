#include "sgementDialog.h"

SegmentDialog::SegmentDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	ui.pInitMethod->addItem("KM_REGION");
	ui.pInitMethod->addItem("GM_PIXEL");
	ui.pInitMethod->addItem("KM_PIXEL");
	ui.pInitMethod->addItem("RAND1");
	ui.pInitMethod->addItem("RAND2");
	ui.pInitInitMethod->addItem("RAND2");
	ui.pInitInitMethod->addItem("RAND1");
}
