#include "FractalWidget.h"


#include <QPainter>
#include <QImage>
#include <QElapsedTimer>
#include <iostream>


FractalWidget::FractalWidget(QWidget* parent) : QWidget(parent),
                                                fractalCreator(WIDTH, HEIGHT),
                                                image(WIDTH, HEIGHT, QImage::Format_RGB32),
                                                transparentLayer(WIDTH, HEIGHT, QImage::Format_ARGB32) {
    setFixedSize(WIDTH, HEIGHT);
    resetTransparentLayer();
    selectionBrush = QBrush(QColor(100, 100, 255, 150), Qt::SolidPattern);

	redrawFractal();
}

QRectF FractalWidget::screenToReal(QRect rectR)
{
	QRectF result(rectR);
	double scaleX = 2 * range / rect().width();
	double scaleY = 2 * range / rect().height();

	QPointF center = result.center();

	center.rx() -= rect().width() / 2;
	center.rx() *= scaleX;

	center.ry() -= rect().height() / 2;
	center.ry() *= (scaleY);


	result.setWidth(result.width() * scaleX);
	result.setHeight(result.height() * (scaleY));
	result.moveCenter(center);

	return result;
}

QPointF FractalWidget::screenToReal(QPoint point)
{
	QPointF result(point);

	double scale = 2 * range / std::min(width(), height());

	result.rx() -= rect().width() / 2;
	result.ry() -= rect().height() / 2;
	result.ry() *= (-1);
	result *= scale;

	result += QPointF { currentXCoord, currentYCoord };

	return result;
}

void FractalWidget::paintEvent(QPaintEvent*) {
	QPainter p(this);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setRenderHint(QPainter::Antialiasing);
	p.drawImage(rect(), image);
	p.drawImage(0, 0, transparentLayer);

}

void FractalWidget::mousePressEvent(QMouseEvent* e) {
    mousePressed = true;
    QPoint position = e->position().toPoint();
    if (e->button() == Qt::RightButton) {
        selectionStart = position;
    }
    if (e->button() == Qt::LeftButton) {
        mouseDragPos = position;

		qDebug() << "On screen: " << position;
		qDebug() << "real:      " << screenToReal(position);
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton && mousePressed) {
        selectionEnd = e->position().toPoint();
        QPoint diff = selectionEnd - selectionStart;
        if (std::abs(diff.x()) < 5 || std::abs(diff.y()) < 5) { // Dont zoom into very small area
            resetTransparentLayer();
            repaint();
            mousePressed = false;

            return;
        }

		QRect selection(selectionStart, selectionEnd);
        
        resetTransparentLayer();
//		redrawFractal();
    }

    mousePressed = false;
}


void FractalWidget::mouseMoveEvent(QMouseEvent* e) {
	QPoint currentMouseLoc = e->position().toPoint();
    if ((e->buttons() & Qt::RightButton) && mousePressed) {
        selectionEnd = currentMouseLoc;
        drawSelection();
    } else if ((e->buttons() & Qt::LeftButton) && mousePressed) {
		QPoint diff = mouseDragPos - currentMouseLoc;

		double scaleX = 2 * range / rect().width();
		double scaleY = 2 * range / rect().height();
		currentXCoord += scaleX * diff.x();
		currentYCoord -= scaleY * diff.y();

        mouseDragPos = currentMouseLoc; // Update position

        redrawFractal();
    }
}

void FractalWidget::wheelEvent(QWheelEvent* e) {
    QPoint numDegrees = e->angleDelta() / 8;
	QPoint d = e->position().toPoint() - rect().center();

    float factor = 0.8f; // Magnification factor
    if (numDegrees.y() < 0) factor = 1.f / factor;
	range *= factor;
	float m = (1-factor) / factor;

	double scaleX = 2 * range / rect().width();
	double scaleY = 2 * range / rect().height();
	currentXCoord += scaleX * m * d.x();
	currentYCoord -= scaleY * m * d.y();

	redrawFractal();
}

void FractalWidget::drawSelection() {
    resetTransparentLayer();
    QPainter painter(&transparentLayer);
    painter.fillRect(QRect(selectionStart, selectionEnd), selectionBrush);
    update();
}

void FractalWidget::resetTransparentLayer() {
    transparentLayer.fill(QColor(0,0,0,0));
}

void FractalWidget::redrawFractal() {
	QElapsedTimer timer;
	timer.start();
	image = fractalCreator.createImageT(QSize{ WIDTH, HEIGHT }, QPointF{ currentXCoord, currentYCoord }, range);


	lastFrameTime = timer.elapsed();
    repaint();
}
