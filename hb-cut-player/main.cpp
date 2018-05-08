#include "XAVIPlayer.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XAVIPlayer w;
    w.show();
    return a.exec();
}
