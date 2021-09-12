#pragma once

#include <QMainWindow>
#include <QLabel>

#include "FractalWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

protected:
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    FractalWidget* w;
    QLabel* mousePosX;
    QLabel* mousePosY;
    QLabel* fractalGenerationTime;
};