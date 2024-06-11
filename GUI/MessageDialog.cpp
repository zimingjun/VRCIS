#include "MessageDialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QPalette>
#include <QProgressBar>
MessageDialog::MessageDialog(QWidget* parent ) :QDialog(parent)
{
    setWindowTitle("Task manager");
    timer0 = new QTimer(this);
    connect(timer0, SIGNAL(timeout()), this, SLOT(timerout()));
    v = new VerDesktop(this);
    ver = v->getVerDesktop();
    messagedialogseting();
}
void MessageDialog::messagedialogseting()
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(240, 248, 255));
    setPalette(palette);
    setAutoFillBackground(true);
    QRect rect = QApplication::desktop()->availableGeometry();
    setGeometry(rect.width() - 500 * ver, rect.height() - 250 * ver, 500 * ver, 250 * ver);
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(10, 10);
    effect->setBlurRadius(10);
    effect->setColor(QColor(50, 50, 50));
    this->setGraphicsEffect(effect);
    imagelabel = new QLabel(this);
    namelabel = new QLabel(this);
    QProgressBar* pProgressBar1 = new QProgressBar(this);
    pProgressBar1->setGeometry(180 * ver, 15 * ver, 300 * ver, 50 * ver);
    QProgressBar* pProgressBar2 = new QProgressBar(this);
    pProgressBar2->setGeometry(180 * ver, 80 * ver, 300 * ver, 50 * ver);
    QProgressBar* pProgressBar3 = new QProgressBar(this);
    pProgressBar3->setGeometry(180 * ver, 145 * ver, 300 * ver, 50 * ver);
    QPropertyAnimation* pAnimation1 = new QPropertyAnimation(pProgressBar1, "value");
    pAnimation1->setDuration(1000);
    pAnimation1->setStartValue(0);
    pAnimation1->setEndValue(100);
    pAnimation1->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* pAnimation2 = new QPropertyAnimation(pProgressBar2, "value");
    pAnimation2->setDuration(2000);
    pAnimation2->setStartValue(0);
    pAnimation2->setEndValue(100);
    pAnimation2->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* pAnimation3 = new QPropertyAnimation(pProgressBar3, "value");
    pAnimation3->setDuration(3000);
    pAnimation3->setStartValue(0);
    pAnimation3->setEndValue(100);
    pAnimation3->start(QAbstractAnimation::DeleteWhenStopped);
    QLabel* namelabel2 = new QLabel(this);
    QLabel* namelabel3 = new QLabel(this);
    namelabel2->setGeometry(0, 80, 150 * ver, 60 * ver);
    namelabel2->setFont(QFont("Î¢ÈíÑÅºÚ", 15 * ver));
    namelabel2->setText("Task_2");
    namelabel2->setAlignment(Qt::AlignCenter);
    namelabel3->setGeometry(0, 145, 150 * ver, 60 * ver);
    namelabel3->setFont(QFont("Î¢ÈíÑÅºÚ", 15 * ver));
    namelabel3->setText("Task_3");
    namelabel3->setAlignment(Qt::AlignCenter);
    namelabel->setGeometry(0, 0, 150 * ver, 60 * ver);
    namelabel->setFont(QFont("Î¢ÈíÑÅºÚ", 15 * ver));
    namelabel->setText("Task_1");
    namelabel->setAlignment(Qt::AlignCenter);
    QPropertyAnimation* animation0 = new QPropertyAnimation(this, "geometry");
    animation0->setDuration(200 * ver);
    animation0->setStartValue(QRect(rect.width() - 100 * ver, rect.height() , 500 * ver, 250 * ver));//Æðµã
    animation0->setEndValue(QRect(rect.width() - 500 * ver, rect.height() - 250 * ver, 500 * ver, 250 * ver));//ÖÕµã
    //animation0->setEasingCurve(QEasingCurve::InBack);
    animation0->start(QAbstractAnimation::DeleteWhenStopped);
    timer0->start(5000);
}
void MessageDialog::timerout()
{
    timer0->stop();
    QPropertyAnimation* animation0 = new QPropertyAnimation(this, "windowOpacity");
    animation0->setDuration(500);
    animation0->setStartValue(1);
    animation0->setEndValue(0);
    animation0->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation0, SIGNAL(finished()), this, SLOT(close()));
}
void MessageDialog::paintEvent(QPaintEvent* event)
{
    const int x = 3;
    Q_UNUSED(event);
    QPainterPath yinying_path;
    yinying_path.setFillRule(Qt::WindingFill);
    yinying_path.addRect(x, x, this->width() - 2 * x, this->height() - 2 * x);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QColor color(0, 0, 0, 50);
    for (int i = 0; i < x; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(x - i, x - i, this->width() - (x - i) * 2, this->height() - (x - i) * 2);
        color.setAlpha(150 - sqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}