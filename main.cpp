#include "include/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style/style.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug() << "open qss success";
        //转换latin1编码，默认utf-8效率慢
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else {
        qDebug() << "open qss failed";
        return 0;
    }

    MainWindow w;
    w.setWindowTitle("ElectronicAlbum");
    w.showMaximized();
    w.show();

    return a.exec();
}
