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
    fractalCreator.calculateIterations2(image);
    repaint();

    
}

void FractalWidget::paintEvent(QPaintEvent*) {
    QPainter qp(this);
    qp.setRenderHint(QPainter::SmoothPixmapTransform);
    qp.setRenderHint(QPainter::Antialiasing);
    qp.drawImage(0, 0, image);
    qp.drawImage(0, 0, transparentLayer);

}

void FractalWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        startX = e->position().x();
        startY = e->position().y();
        // fractalCreator.xStart = fractalCreator.screenToRealX(startX);
        // fractalCreator.yStart = fractalCreator.screenToRealY(startY);
        mousePressed = true;
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton && mousePressed) {
        mousePressed = false;
        lastX = e->position().x();
        lastY = e->position().y();
        double sx = fractalCreator.screenToRealX(startX);
        double ex = fractalCreator.screenToRealX(lastX);
        double sy = fractalCreator.screenToRealY(startY);
        double ey = fractalCreator.screenToRealY(lastY);
        double r = (ey - sy)/2;
        double x = (ex + sx) /2;
        double y = (ey + sy) /2;
        fractalCreator.range = std::abs(r);
        fractalCreator.cX = x;
        fractalCreator.cY = y;
        
        // fractalCreator.xEnd = fractalCreator.screenToRealX(lastX);
        // fractalCreator.yEnd = fractalCreator.screenToRealY(lastY);
        fractalCreator.calculateIterations2(image);
        resetTransparentLayer();
        repaint();
    }
}

void FractalWidget::mouseMoveEvent(QMouseEvent* e) {
    if ((e->buttons() & Qt::RightButton) && mousePressed) {
        lastX = e->position().x();
        lastY = e->position().y();
        drawSelection();
    } 
}

void FractalWidget::drawSelection() {
    resetTransparentLayer();
    QPainter painter(&transparentLayer);
    painter.fillRect(startX, startY, lastX - startX, lastY - startY, selectionBrush);
    update();
}

void FractalWidget::resetTransparentLayer() {
    transparentLayer.fill(QColor(0,0,0,0));
}