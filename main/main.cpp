#include <QApplication>
#include "MainWindow.h"

#include <iostream>




int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

	MainWindow w;
	w.show();

    return app.exec();
}
