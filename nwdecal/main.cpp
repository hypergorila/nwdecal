#include "nwdecal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NWDECal w;
    w.setWindowTitle("NW DECal - Neverwinter Debuff Effectivness Calculator 1.0a1");
    w.show();

    return a.exec();
}
