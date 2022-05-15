#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include "gui/dialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Dialog dialog;
    dialog.show();

    return QApplication::exec();
}
