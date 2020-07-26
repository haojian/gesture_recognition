#include "gestureRender.h"
#include <QPoint>

Render_Gesture::Render_Gesture():
    featurePoints(0),
    render_type(EGES_INVALID)
{
    render_bbox = new BoundingBox();
    reset();
}

Render_Gesture::~Render_Gesture()
{
    featurePoints.clear();
}

void Render_Gesture::reset()
{
    featurePoints.clear();
    render_timestamp.clear();
    render_bbox->reset();
    render_type = EGES_INVALID;
    radius = 0;
}

void Render_Gesture::draw()
{
    QImage renderCanvas(728,412, QImage::Format_ARGB32);
    renderCanvas.fill(qRgba(255,255,255, 0));

    QPainter pp(&renderCanvas);
    QPen penRender = QPen();
    penRender.setColor(Qt::red);
    penRender.setStyle(Qt::SolidLine);
    penRender.setWidth(4);
    pp.setPen(penRender);

    switch(render_type)
    {
    case EGES_POINT:
        qDebug("Point Rendered");
        break;
    case EGES_LINE:
        if(featurePoints.size() == 2)
        {
            QPoint p1 = QPoint(featurePoints[0].x, featurePoints[0].y);
            QPoint p2 = QPoint(featurePoints[1].x, featurePoints[1].y);
            pp.drawLine(p1,p2);
            featurePoints.clear();
        }
        else
            qDebug("Line Render Error");
        break;
    case EGES_CIRCLE:
        if(featurePoints.size() == 1 && radius != 0)
        {
            pp.drawEllipse(QPoint(featurePoints[0].x, featurePoints[0].y), radius, radius);
        }
        else
            qDebug("Circle Render Error");
        break;
    case EGES_ERASE:
        if(featurePoints.size() == 2)
        {
            int width = featurePoints[1].x - featurePoints[0].x;
            int height = featurePoints[1].y - featurePoints[0].y;
            pp.drawRect(featurePoints[0].x, featurePoints[0].y, width, height);
            QPoint p1 = QPoint(featurePoints[0].x, featurePoints[0].y);
            QPoint p2 = QPoint(featurePoints[1].x, featurePoints[1].y);
            QPoint p3 = QPoint(featurePoints[1].x, featurePoints[0].y);
            QPoint p4 = QPoint(featurePoints[0].x, featurePoints[1].y);

            pp.drawLine(p1, p2);
            pp.drawLine(p3, p4);
        }
        else
            qDebug("Eraze Render Error");
        break;
    case EGES_POLYLINE:
        qDebug("Num: %d", featurePoints.size());
        if(featurePoints.size() > 2)
        {
            for(int i = 0; i< featurePoints.size(); i++)
            {
                qDebug("X= %f, Y= %f", featurePoints[i].x, featurePoints[i].y) ;
            }


            int points_num = featurePoints.size();
            QPoint * point_arr = new QPoint[points_num];
            for(int i = 0; i< featurePoints.size(); i++)
            {
                point_arr[i]  = QPoint(featurePoints[i].x, featurePoints[i].y);
            }
            pp.drawPolyline(point_arr, points_num);
            delete [] point_arr;
        }
        else
            qDebug("Polyline Render Error");
        break;
    default:
        reset();
    }

    emit theImage(renderCanvas);
    emit renderType(this->render_type);

}



