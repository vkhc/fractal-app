#pragma once

#include <QWidget>
#include <QMouseEvent>

#include "FractalImageCreator.h"


class FractalWidget : public QWidget {
    Q_OBJECT
public:
    


    FractalWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    void drawSelection();
    void resetTransparentLayer();

    const int WIDTH = 800;  // Order matters!
    const int HEIGHT = 600; // Initialize scrren size before FractalImageCreator
    FractalImageCreator fractalCreator;

    bool mousePressed = false;

    QImage image;            // Fractal is drawn here
    QImage transparentLayer; // Transparent layer to draw
                             // selection rectange
    QPoint selectionStart;
    QPoint selectionEnd;
    QBrush selectionBrush;   // Brush for selection rectangle
};