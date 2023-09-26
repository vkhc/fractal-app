#include "FractalWidget.h"


#include <QPainter>
#include <QImage>
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

void FractalWidget::paintEvent(QPaintEvent*) {
	QPainter p(this);
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.setRenderHint(QPainter::Antialiasing);
	p.drawImage(0, 0, image);
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
        fractalCreator.setDrawingArea(selectionStart.x(), selectionStart.y(),
                                   selectionEnd.x(), selectionEnd.y());
        
        resetTransparentLayer();
        redrawFractal();
    }
    mousePressed = false;
}


void FractalWidget::mouseMoveEvent(QMouseEvent* e) {
    QPoint currentMouseLoc = e->position().toPoint();
    currentXCoord = fractalCreator.screenToRealX(currentMouseLoc.x());
    currentYCoord = fractalCreator.screenToRealY(currentMouseLoc.y());
    if ((e->buttons() & Qt::RightButton) && mousePressed) {
        selectionEnd = currentMouseLoc;
        drawSelection();
    } else if ((e->buttons() & Qt::LeftButton) && mousePressed) {
        QPoint diff = mouseDragPos - currentMouseLoc;
        mouseDragPos = currentMouseLoc; // Update position
        fractalCreator.moveImageCenter(diff.x(), diff.y());
        redrawFractal();
    }
}

void FractalWidget::wheelEvent(QWheelEvent* e) {
    QPoint numDegrees = e->angleDelta() / 8;
    QPoint d = e->position().toPoint() - QPoint(WIDTH/2 , HEIGHT/2);

    float factor = 0.8f; // Magnification factor
    if (numDegrees.y() < 0) factor = 1.f / factor;
    fractalCreator.adjustRange(factor);
    float m = (1-factor) / factor;
    fractalCreator.moveImageCenter(m * d.x(), m * d.y());
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
    fractalCreator.calculateIterationsThread(image);
    lastFrameTime = fractalCreator.frameCalcTime;
    repaint();
}
