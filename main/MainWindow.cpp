#include "MainWindow.h"

#include <QStatusBar>
#include <thread>

MainWindow::MainWindow(): w(new FractalWidget(this)) {
	setCentralWidget(w);
	setMouseTracking(true);                     // Both of these lines needed
	centralWidget()->setMouseTracking(true);    // to capture mouse move
	w->installEventFilter(this);                // in main window

	auto threadInfo = new QLabel(this);
	int nThreads = std::thread::hardware_concurrency();
	QString text = QString("%1 Threads available ").arg(nThreads);
	threadInfo->setAlignment(Qt::AlignRight);
	threadInfo->setText(text);
    
	mousePosX = new QLabel("X=0.0", this);
	mousePosY = new QLabel("Y=0.0", this);
	fractalGenerationTime = new QLabel(this);

	QString temp = QString("Image generated in %1 s").arg(w->lastFrameTime);
	fractalGenerationTime->setAlignment(Qt::AlignRight);
	fractalGenerationTime->setText(temp);

	statusBar()->addWidget(mousePosX, 4);
	statusBar()->addWidget(mousePosY, 4);
	statusBar()->addWidget(threadInfo, 5);
	statusBar()->addWidget(fractalGenerationTime, 6);
}


bool MainWindow::eventFilter(QObject* object, QEvent* event) {
	if (event->type() == QEvent::MouseMove) {
		QString temp = QString("X=%1").arg(QString::number(w->origin.x(),'g', 16));
		mousePosX->setText(temp);
		temp = QString("Y=%1").arg(QString::number(w->origin.y(),'g', 16));
		mousePosY->setText(temp);
	} if (event->type() == QEvent::Paint) {
		QString temp = QString("Image generated in %1 ms ").arg(w->lastFrameTime);
		fractalGenerationTime->setText(temp);
	}
	return false;
}
