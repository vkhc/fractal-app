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
    fractalCreator.calculateIterationsThread(image);
    repaint();

    
}

void FractalWidget::paintEvent(QPaintEvent*) {
    QPainter qp(this);
    qp.setRenderHint(QPainter::SmoothPixmapTransform);
    qp.setRenderHint(QPainter::Antialiasing);
    qp.drawImage(0, 0, image);
    qp.drawImage(0, 0, transparentLayer);

    displayCalcTime(qp);
}

void FractalWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        selectionStart = e->position().toPoint();

        mousePressed = true;
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton && mousePressed) {
        mousePressed = false;
        selectionEnd = e->position().toPoint();
        fractalCreator.setDrawingArea(selectionStart.x(), selectionStart.y(),
                                   selectionEnd.x(), selectionEnd.y());
        
        fractalCreator.calculateIterationsThread(image);
        resetTransparentLayer();
        repaint();
    }
}

void FractalWidget::wheelEvent(QWheelEvent* e) {
    QPoint numDegrees = e->angleDelta() / 8;
    QPoint mousePos = e->position().toPoint();
    float factor = 0.8f;
    if (numDegrees.y() < 0) factor = 1.f / factor;
    fractalCreator.adjustRange(factor);
    fractalCreator.setImageCenter(mousePos.x(), mousePos.y());
    fractalCreator.calculateIterationsThread(image);
    repaint();
}

void FractalWidget::mouseMoveEvent(QMouseEvent* e) {
    if ((e->buttons() & Qt::RightButton) && mousePressed) {
        selectionEnd = e->position().toPoint();
        drawSelection();
    } 
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

void FractalWidget::displayCalcTime(QPainter& p) {
    // Display fractal generation time
    QFont font = p.font();
    font.setPixelSize(36);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(4,40, QString::number(fractalCreator.getElapsedTime()));
}