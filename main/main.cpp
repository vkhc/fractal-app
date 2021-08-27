#include <QApplication>
#include "FractalWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    FractalWidget w;
    w.show();

    return app.exec();
}