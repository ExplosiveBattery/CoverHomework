#include "MainWindow.h"
#include "LoginWindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    w.show();


    return a.exec();
}

