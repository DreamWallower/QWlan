#include "QWlan.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/QWlan/Resources/logo.ico"));
    QWlan w;
    w.show();
    return a.exec();
}
