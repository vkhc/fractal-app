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

QPointF FractalWidget::screenToReal(QPoint point)
{
	QPointF result(point);

	result.rx() -= rect().width() / 2;
	result.ry() -= rect().height() / 2;
	result.ry() *= (-1);
	result *= scaleFactor();

	result += origin;

	return result;
}

void FractalWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setRenderHint(QPainter::Antialiasing);
	p.drawImage(rect(), image);

	drawSelection(p);

}

void FractalWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton)
	{
		selection.setTopLeft(e->position().toPoint());
    }
	if (e->button() == Qt::LeftButton)
	{
		dragStartPos = e->position().toPoint();
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent* e) {
	if (e->button() == Qt::RightButton)
	{
		selection.setBottomRight(e->position().toPoint());
		if (!selection.isValid())
			selection = selection.normalized();

		if (selection.width() * selection.height() > 100)
		{ // Dont zoom into very small area

			origin = screenToReal(selection.center());
			range = std::max(selection.width(), selection.height()) * scaleFactor() / 2;

			regenerateImage();
			selection = QRect();
		}

		update();
	}
}

void FractalWidget::mouseMoveEvent(QMouseEvent* e)
{
	QPoint currentPos = e->position().toPoint();
	if ((e->buttons() & Qt::RightButton))
	{
		selection.setBottomRight(currentPos);

		update();
	}
	else if ((e->buttons() & Qt::LeftButton))
	{
		QPoint d = dragStartPos - currentPos;

		double scale = 2 * range / std::min(width(), height());
		origin.rx() += scale * d.x();
		origin.ry() -= scale * d.y();

		dragStartPos = currentPos; // Update position

		regenerateImage();
		update();
    }
}

void FractalWidget::wheelEvent(QWheelEvent* e)
{
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

void FractalWidget::drawSelection(QPainter& p)
{
	if (selection.isNull())
		return;

	QBrush selectionBrush(QColor(100, 100, 255, 150), Qt::SolidPattern);
	p.fillRect(selection, selectionBrush);
}

void FractalWidget::regenerateImage()
{
	QElapsedTimer timer;
	timer.start();

	image = fractalCreator.createImageT(QSize{ WIDTH, HEIGHT } , origin, range);

	lastFrameTime = timer.elapsed();
}
