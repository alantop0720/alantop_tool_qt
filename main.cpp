#include "widget.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QRect rect = w.frameGeometry();
    QDesktopWidget desktop;
    QPoint centerPoint = desktop.availableGeometry().center();
    rect.moveCenter(centerPoint);
    w.move(rect.topLeft());

    w.show();



    return a.exec();
}
