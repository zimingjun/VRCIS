#pragma once
#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H
#include <QDialog>
#include <QPropertyAnimation>
#include <QLabel>
#include "verdesktop.h"
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QHBoxLayout>

class MessageDialog : public QDialog
{
	Q_OBJECT
public:
	float ver;
	VerDesktop* v;
	explicit MessageDialog(QWidget* parent = nullptr);
	QLabel* imagelabel;
	QLabel* namelabel;
	void messagedialogseting();
	QTimer* timer0;
    QLabel *title_label;
    
    QHBoxLayout *layout ;
    
private:
	void paintEvent(QPaintEvent* event);

public slots:
	void timerout();
};

#endif // MESSAGEDIALOG_H