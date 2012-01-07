#ifndef GESTURERENDER_H
#define GESTURERENDER_H

#include <QPainter>
#include "gesture_DS.h"

//abstract base class for Gesture

class Render_Gesture : public QObject
{
    Q_OBJECT

public:
    Render_Gesture();
    ~Render_Gesture();

    void reset();

    PosList featurePoints;
    std::string render_timestamp;
    E_GESTURE_TYPE  render_type;
    BoundingBox *render_bbox;
    int radius;

//    QPixmap pixmap;
    void draw();
//    QPixmap renderView();

signals:
     void theImage(const QImage &);
     void renderType(E_GESTURE_TYPE type);
};
#endif // GESTURERENDER_H
