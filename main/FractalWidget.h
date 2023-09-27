#pragma once

#include <QWidget>
#include <QMouseEvent>

#include "FractalImageCreator.h"


class FractalWidget : public QWidget {
    Q_OBJECT
public:

    FractalWidget(QWidget* parent = nullptr);
	QPointF origin = { -0.6, 0.0 };
	double range = 1.0;

	double lastFrameTime = 0.0;

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
	void drawSelection(QPainter& p);
	void regenerateImage();

	QRectF screenToReal(QRect point);
	QPointF screenToReal(QPoint point);

	double scaleFactor() { return 2 * range / std::min(width(), height()); }

private:
	FractalImageCreator fractalCreator;

	bool leftButtonClicked = false;
	bool rightButtonClicked = false;

	QImage image;
	QRect selection;
    QPoint mouseDragPos;
};
