#include "SkyWheelWindow.h"

#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[]){

    // create the application
    QApplication app(argc, argv);

    // create a master widget
    SkyWheelWindow *window = new SkyWheelWindow(NULL);
    window->ResetInterface();

    // resize the window
    window->resize(700, 900);

    // show the label
    window->show();

    // start it running
    app.exec();

    // clean up
    delete window;

    // return to caller
    return 0;
}
