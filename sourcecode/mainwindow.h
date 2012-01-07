#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "gestureAlgorithm.h"
#include "gesture_DS.h"
#include "gestureRender.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void paint(QImage& theImage);
    void renderGesture(QImage& theImage);
    void finished();
    void tmpDataReset();

private:
    Ui::MainWindow *ui;
    QPoint lastPos, curPos;
    QImage tempImage, image;

    QColor penColor;
    QColor brushColor;
    QRgb backColor;
    Qt::PenStyle penStyle;

    GestureAlgorithm recognizer;
    Render_Gesture *curGestureRender;

private slots:
    void on_actionClear_triggered();
    void displayImage(const QImage &);
    void resultUpdate(E_GESTURE_TYPE type);
};

#endif // MAINWINDOW_H
