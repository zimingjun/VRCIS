#include "UIMain.h"
#include <QtCore/QTextCodec>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QFont>
#include <QPainter>
int main( int argc, char *argv[] )
{
    QApplication a(argc, argv);

    QSplashScreen splash(QPixmap(":/GUI/imageSources/vrcis.png"));
    splash.show();
    QTimer::singleShot(5000, &splash, &QWidget::close);
    /*QTimer::singleShot(2000, &splash, &QSplashScreen::close);
    QThread::sleep(2);*/
    
    QTextCodec *codec = QTextCodec::codecForName( "GBK" );
    if( codec == NULL )
        codec = QTextCodec::codecForLocale();
        
    //QTextCodec::setCodecForLocale( codec );
    //QTextCodec::setCodecForTr( codec );
    //QTextCodec::setCodecForCStrings( codec );
    
    UIImgTest w;
    w.show();
    return a.exec();
}
