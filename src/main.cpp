#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

  //  QFile f(":light/style.qss");
    QFile f(":dark/style.qss");
    if (!f.exists())   {
        qDebug()<<"Unable to set stylesheet, file not found\n"<<f.fileName();
    }

    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setStyleSheet(ts.readAll());
    }


    MainWindow w;
    w.show();
    return a.exec();
}
















































