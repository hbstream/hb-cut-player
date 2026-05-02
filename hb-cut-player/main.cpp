#include "XAVIPlayer.h"

#include <QApplication>
#include <QStyleHints>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("HB Cut Player");
    QApplication::setOrganizationName("hbstream");

    XAVIPlayer w;
    w.show();
    return a.exec();
}
