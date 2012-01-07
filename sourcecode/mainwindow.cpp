#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recognizer = GestureAlgorithm(5, 0.9);

    curGestureRender =new Render_Gesture();
    recognizer.curGestureRender = curGestureRender;

    image = QImage(728,412,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);
    image.fill(backColor);

    penColor = Qt::black;
    brushColor = Qt::black;
    penStyle = Qt::SolidLine;

    ui->gesture_TypeDisplay->setText("Gesture INVALID");
    connect(curGestureRender, SIGNAL(theImage(QImage)), this, SLOT(displayImage(QImage)));
    connect(curGestureRender, SIGNAL(renderType(E_GESTURE_TYPE)), this, SLOT(resultUpdate(E_GESTURE_TYPE)));
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::finished()
{

}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        curPos = e->pos();
        lastPos = curPos;
        paint(image);

        recognizer.startGesture(e->pos().x(),e->pos().y());

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        curPos = e->pos();
        paint(image);

        recognizer.addPoint(e->pos().x(),e->pos().y());
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        lastPos = e->pos();
        paint(image);

        recognizer.endGesture(e->pos().x(),e->pos().y());
        image.fill(backColor);
        tmpDataReset();
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
    painter.drawImage(0,0, tempImage);
}

void MainWindow::paint(QImage &theImage)
{
    QPainter pp(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(4);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    pp.drawLine(lastPos, curPos);
    lastPos = curPos;

    update();
}


void MainWindow::renderGesture(QImage &theImage)
{
    QPainter pp(&theImage);
    QPen penRender = QPen();
    penRender.setColor(Qt::red);
    penRender.setStyle(penStyle);
    penRender.setWidth(4);
    QBrush brushRender = QBrush(brushColor);
    pp.setPen(penRender);
    pp.setBrush(brushRender);
    update();
}

void MainWindow::on_actionClear_triggered()
{
    ui->gesture_TypeDisplay->setText("Gesture INVALID");
    image.fill(backColor);
    tempImage.fill(qRgba(255,255,255, 0));
    update();
    recognizer.reset();
    curGestureRender->reset();
}

void MainWindow::tmpDataReset()
{
    image.fill(backColor);
//    tempImage.fill(qRgba(255,255,255, 0));
    update();
    recognizer.reset();
    curGestureRender->reset();
}

void MainWindow::displayImage(const QImage &theImage)
{
    qDebug("Succeed");
    tempImage = theImage;
    update();
}

void MainWindow::resultUpdate(E_GESTURE_TYPE type)
{
    switch(type)
    {
    case EGES_INVALID:
        ui->gesture_TypeDisplay->setText("INVALID");
        break;
    case EGES_POINT:
        ui->gesture_TypeDisplay->setText("Point Recognized");
        break;
    case EGES_LINE:
        ui->gesture_TypeDisplay->setText("Line Recognized");
        break;
    case EGES_CIRCLE:
        ui->gesture_TypeDisplay->setText("Circle Recognized");
        break;
    case EGES_ERASE:
        ui->gesture_TypeDisplay->setText("ERASE Recognized \n selected region would be erased.");
        break;
    case EGES_POLYLINE:
        ui->gesture_TypeDisplay->setText("Polyline Recognized");
        break;
    default:
        break;
    }

}

