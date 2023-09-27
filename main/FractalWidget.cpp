#include "FractalWidget.h"

#include <QPainter>
#include <QImage>
#include <QElapsedTimer>
#include <iostream>

constexpr int WIDTH = 600;
constexpr int HEIGHT = 600;

FractalWidget::FractalWidget(QWidget* parent) : QWidget(parent),
												fractalCreator(),
												image(WIDTH, HEIGHT, QImage::Format_RGB32)
{
    setFixedSize(WIDTH, HEIGHT);

	regenerateImage();
}

QRectF FractalWidget::screenToReal(QRect rectR)
{
	QRectF result(rectR);
	double scale = 2 * range / std::min(width(), height());

	QPointF center = result.center();

	center.rx() -= rect().width() / 2;
	center.rx() *= scale;

	center.ry() -= rect().height() / 2;
	center.ry() *= (scale);


	result.setWidth(result.width() * scale);
	result.setHeight(result.height() * (scale));
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

	result += origin;

	return result;
}

void FractalWidget::paintEvent(QPaintEvent*) {
	QPainter p(this);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setRenderHint(QPainter::Antialiasing);
	p.drawImage(rect(), image);

	if (rightButtonClicked)
		drawSelection(p);

}

void FractalWidget::mousePressEvent(QMouseEvent* e) {
    QPoint position = e->position().toPoint();
    if (e->button() == Qt::RightButton) {
		rightButtonClicked = true;
		selection.setTopLeft(position);
    }
    if (e->button() == Qt::LeftButton) {
		leftButtonClicked = true;
        mouseDragPos = position;

		qDebug() << "On screen: " << position;
		qDebug() << "real:      " << screenToReal(position);
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent* e) {
	if (e->button() == Qt::RightButton && rightButtonClicked) {
		selection.setBottomRight(e->position().toPoint());
		rightButtonClicked = false;

        
		if (selection.width() * selection.height() > 100) { // Dont zoom into very small area
			origin = screenToReal(selection.center());
			range = std::max(selection.width(), selection.height()) * scaleFactor() / 2;

			regenerateImage();
		}

		update();

	} else if (e->button() == Qt::LeftButton && leftButtonClicked) {
		leftButtonClicked = false;
	}
}

void FractalWidget::mouseMoveEvent(QMouseEvent* e) {
	QPoint currentMouseLoc = e->position().toPoint();
	if ((e->buttons() & Qt::RightButton) && rightButtonClicked) {
		selection.setBottomRight(currentMouseLoc);
		update();
	} else if ((e->buttons() & Qt::LeftButton) && leftButtonClicked) {
		QPoint diff = mouseDragPos - currentMouseLoc;

		double scale = 2 * range / std::min(width(), height());
		origin.rx() += scale * diff.x();
		origin.ry() -= scale * diff.y();

        mouseDragPos = currentMouseLoc; // Update position

		regenerateImage();
		update();
    }
}

void FractalWidget::wheelEvent(QWheelEvent* e) {
    QPoint numDegrees = e->angleDelta() / 8;
	QPoint d = e->position().toPoint() - rect().center();

    float factor = 0.8f; // Magnification factor
    if (numDegrees.y() < 0) factor = 1.f / factor;
	range *= factor;
	float m = (1-factor) / factor;

	double scale = 2 * range / std::min(width(), height());
	origin.rx() += scale * m * d.x();
	origin.ry() -= scale * m * d.y();

	regenerateImage();
	update();
}

void FractalWidget::drawSelection(QPainter& p) {
	QBrush selectionBrush(QColor(100, 100, 255, 150), Qt::SolidPattern);
	p.fillRect(selection, selectionBrush);
}

void FractalWidget::regenerateImage() {
	QElapsedTimer timer;
	timer.start();
	image = fractalCreator.createImageT(QSize{ WIDTH, HEIGHT }, origin, range);

	lastFrameTime = timer.elapsed();
}
