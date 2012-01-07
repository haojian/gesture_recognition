#include "gestureAlgorithm.h"
#include <QDebug>

GestureAlgorithm::GestureAlgorithm(int minimumMovement, double minimumMatch):
        midPoint(0,0),
        mainDirections(0),
        positions(0)
{
    minMatch = minimumMatch;
    minMovement2= minimumMovement * minimumMatch;

    relativeIgnoreFactor = 0.5;
    absoluteIgnoreFactor = 5;
    this->reset();
}

GestureAlgorithm::~GestureAlgorithm()
{
    reset();
}

void GestureAlgorithm::reset()
{
    positions.clear();
    bIsPoint = true;
    dist_sum = 0;
    dist_average = 0;
    point_num = 0;

    theta_sum =0; theta_average =0; theta_sqsum =0; theta_factor = 0;
    upNum = 0; downNum = 0; leftNum = 0; rightNum =0;
    pos_x_sum = 0; pos_y_sum = 0; pos_xx_sum = 0; pos_xy_sum = 0;
}

void GestureAlgorithm::addPoint(int x, int y)
{
    int d_x, d_y;

    d_x = x-positions.back().x;
    d_y = y-positions.back().y;

    if( d_x*d_x + d_y*d_y >= 5 )
    {
        updateStatistic(x, y);
        recognizeGesture();
    }
}

void GestureAlgorithm::startGesture(int x, int y)
{
    positions.clear();
    updateStatistic(x, y);
//    positions.push_back(Point(x,y));

}

void GestureAlgorithm::endGesture(int x, int y)
{
    if(x != positions.back().x || y != positions.back().y)
        updateStatistic(x, y);
//        positions.push_back(Point(x,y));

//    recognizeGesture();

    positions.clear();
}

void GestureAlgorithm::recognizeGesture()
{
    for(int i = 0; i< mainDirections.size(); i++)
    {
        qDebug() << "X= " << mainDirections[i].x << "Y=" << mainDirections[i].y;
    }
    qDebug() << "Directions Number: " << mainDirections.size();
    qDebug() << "Directions Length: " << calcLength( mainDirections );
    qDebug() << "UP Number: " << upNum << "Down Number:" << downNum  ;
    qDebug() << "Left: "<< leftNum <<"right" << rightNum;
    qDebug() << "Position Num: "<< positions.size();

    if(bIsPoint && isPoint())
    {
        qDebug() << "Point Recognized: ";
        curGestureRender->render_type = EGES_POINT;
        curGestureRender->featurePoints.clear();
        curGestureRender->draw();
        bIsShapeFixed= false;
    }

    else if (bIsShapeFixed && curGestureRender->render_type == EGES_ERASE)
    {
        curGestureRender->render_type = EGES_ERASE;
        curGestureRender->featurePoints.clear();
        curGestureRender->featurePoints.push_back(curGestureRender->render_bbox->getMinCornerPoint());
        curGestureRender->featurePoints.push_back(curGestureRender->render_bbox->getMaxCornerPoint());
        curGestureRender->draw();
        bIsShapeFixed = true;
    }
    else if(isCircle())
    {
        qDebug() << "Circle Recognized: ";
        curGestureRender->render_type = EGES_CIRCLE;
        curGestureRender->featurePoints.clear();
        curGestureRender->featurePoints.push_back(curGestureRender->render_bbox->getCenter());
        curGestureRender->radius = (int)getRadius();
        curGestureRender->draw();
        bIsShapeFixed= false;
    }
    else if(isEraze())
    {
        qDebug() << "Eraze Recognized: ";
        curGestureRender->render_type = EGES_ERASE;
        curGestureRender->featurePoints.clear();
        curGestureRender->featurePoints.push_back(curGestureRender->render_bbox->getMinCornerPoint());
        curGestureRender->featurePoints.push_back(curGestureRender->render_bbox->getMaxCornerPoint());
        curGestureRender->draw();
        bIsShapeFixed = true;
    }
    else if(isStraightLine())
    {
        qDebug() << "Line Recognized: ";
        curGestureRender->render_type = EGES_LINE;
        curGestureRender->featurePoints.clear();
        float a,b;
        if(calculateLine(a,b))
        {
            Point v0 = Point(1,b);
            normalize(v0);
            float maxd = std::numeric_limits<float>::min();
            float mind = std::numeric_limits<float>::max();

            for( int i = 0; i < positions.size(); i++)
            {
                float d = dot(v0, Point(positions[i].x, positions[i].y -a));
                maxd = max2( maxd, d);
                mind = min2( mind, d);
            }
            Point begin = Point(v0.x * mind, v0.y * mind);
            Point end = Point(v0.x * maxd, v0.y * maxd);

            curGestureRender->featurePoints.push_back(Point(begin.x, a + begin.x * b));
            curGestureRender->featurePoints.push_back(Point(end.x, a+ end.x *b));
        }
        else
        {
            curGestureRender->featurePoints.push_back(Point(midPoint.x, curGestureRender->render_bbox->getMinCornerPoint().y));
            curGestureRender->featurePoints.push_back(Point(midPoint.x, curGestureRender->render_bbox->getMaxCornerPoint().y));
        }
        curGestureRender->draw();
        bIsShapeFixed= false;
    }
    else if(isPolyline())
    {
        qDebug() << "Polyline Recognized: ";
        curGestureRender->render_type = EGES_POLYLINE;
        curGestureRender->featurePoints.clear();
        curGestureRender->featurePoints.push_back(positions[0]);
        int j=0;
        for(int i = 0; i< mainDirections.size(); i++)
        {
//            qDebug() << "X= " << mainDirections[i].x << "Y=" << mainDirections[i].y;
            if(mainDirections[i].x != 0 && mainDirections[i].y == 0)
            {
                int dist = mainDirections[i].x;
                for(; j <positions.size(); j++)
                {
                    if(abs(positions[j].x - curGestureRender->featurePoints.back().x) > abs(mainDirections[i].x) - 10)
                    {
                        curGestureRender->featurePoints.push_back(positions[j]);
                        break;
                    }
                }
            }
            else if(mainDirections[i].x == 0 && mainDirections[i].y != 0)
            {
                int dist = mainDirections[i].y;
                for(; j <positions.size(); j++)
                {
                    if(abs(positions[j].y - curGestureRender->featurePoints.back().y) > abs(mainDirections[i].y) -10)
                    {
                        curGestureRender->featurePoints.push_back(positions[j]);
                        break;
                    }
                }
            }
        }
        curGestureRender->featurePoints.push_back(positions.back());
        curGestureRender->draw();
        bIsShapeFixed= false;
    }
    else
    {
        qDebug()<< "Not Rcognized";
    }
}

int GestureAlgorithm::calcLength(const PosList &positions)
{
    int res = 0;
    for(int index =0; index < positions.size(); ++index)
    {
        if(positions[index].x > 0)
            res += positions[index].x;
        else if(positions[index].x < 0)
            res -= positions[index].x;
        else if(positions[index].y >0)
            res += positions[index].y;
        else
            res -= positions[index].y;
    }
    return res;
}

PosList GestureAlgorithm::limitDirections(const PosList &positions)
{
    PosList res;
    int lastx, lasty;
    bool firstTime = true;

    for( PosList::const_iterator ii = positions.begin(); ii != positions.end(); ++ii )
    {
        if( firstTime )
        {
            lastx = ii->x;
            lasty = ii->y;

            firstTime = false;
        }
        else
        {
            int dx, dy;

            dx = ii->x - lastx;
            dy = ii->y - lasty;

            if( dy > 0 )
            {
                if( dx > dy || -dx > dy )
                    dy = 0;
                else
                    dx = 0;
            }
            else
            {
                if( dx > -dy || -dx > -dy )
                    dy = 0;
                else
                    dx = 0;
            }
            res.push_back( Point( dx, dy ) );
            lastx = ii->x;
            lasty = ii->y;
        }
    }

    return res;
}

PosList GestureAlgorithm::simplify(const PosList &positions)
{
    PosList res;
    int lastdx = 0, lastdy = 0;
    bool firstTime = true;

    int index=0;
    for( PosList::const_iterator ii = positions.begin(); ii != positions.end(); ++ii )
    {
        if( firstTime )
        {
            lastdx = ii->x;
            lastdy = ii->y;

            firstTime = false;
        }
        else
        {

            bool joined = false;

            if( (lastdx > 0 && ii->x > 0) || (lastdx < 0 && ii->x < 0) )
            {
                lastdx += ii->x;
                joined = true;
            }

            if( (lastdy > 0 && ii->y > 0) || (lastdy < 0 && ii->y < 0) )
            {
                lastdy += ii->y;
                joined = true;
            }

            if( !joined )
            {
                res.push_back( Point( lastdx, lastdy ) );

                lastdx = ii->x;
                lastdy = ii->y;
            }
        }
    }

    if( lastdx != 0 || lastdy != 0 )
    {
        res.push_back( Point( lastdx, lastdy ) );
    }

    return res;
}

PosList GestureAlgorithm::removeShortestNoise(const PosList &positions)
{
    PosList res;
    int shortestSoFar;
    PosList::const_iterator shortest;
    bool firstTime = true;

    for( PosList::const_iterator ii = positions.begin(); ii != positions.end(); ++ii )
    {
        if( firstTime )
        {
            shortestSoFar = ii->x*ii->x + ii->y*ii->y;
            shortest = ii;

            firstTime = false;
        }
        else
        {
            if( (ii->x*ii->x + ii->y*ii->y) < shortestSoFar )
            {
                shortestSoFar = ii->x*ii->x + ii->y*ii->y;
                shortest = ii;
            }
        }
    }

    for( PosList::const_iterator ii = positions.begin(); ii != positions.end(); ++ii )
    {
        if( ii != shortest)
            res.push_back( *ii );
    }

    return res;
}

bool GestureAlgorithm::isCircle()
{
    if(upNum >=1 && downNum >= 1 && leftNum >= 1 && rightNum >= 1 || ((upNum + downNum+ leftNum+ rightNum) == 3 && upNum < 2 && downNum <2 && leftNum <2 && rightNum <2))
    {
        return true;
    }
    //Patch for Ellipse
    qDebug("xY: %f, %f,", positions.begin()->x, positions.begin()->y);
    qDebug("xY: %f, %f,", positions.back().x, positions.back().y);
    qDebug("dist: %f, average: %f", positions.begin()->dist(positions.back().x, positions.back().y), dist_average);
    if(((sign(upNum) + sign(downNum)) == 2 || (sign(leftNum) + sign(rightNum)) == 2) && positions.begin()->dist(positions.back().x, positions.back().y) < 0.5 * dist_average)
    {
        return true;
    }
    return false;
}

bool GestureAlgorithm::isPolyline()
{
    if((abs(sign(upNum)) + abs(sign(downNum))+ abs(sign(leftNum))+ abs(sign(rightNum))) <=3)
    {
        return true;
    }
    return false;
}

bool GestureAlgorithm::isStraightLine()
{
    qDebug("theta factor: %f", theta_factor);
    if(theta_factor <0.1f || (upNum + downNum+ leftNum+ rightNum) == 1)
    {
        lastGestureType = curGestureType;
        curGestureType = EGES_LINE;
        return true;
    }
    return false;

}

bool GestureAlgorithm::isEraze()
{
    if((upNum + downNum) > 2 && leftNum ==0 && rightNum ==0)
    {
        return true;
    }
    else if((leftNum + rightNum) > 2 && upNum ==0 && downNum ==0)
    {
        return true;
    }
    else if((upNum + downNum+ leftNum+ rightNum) > 8)
    {
        return true;
    }
    return false;
}


bool GestureAlgorithm::isPoint()
{
  if(dist_average < 15)
      bIsPoint = true;
  else
      bIsPoint = false;
  return bIsPoint;
}


PosList GestureAlgorithm::detectDirection(const PosList &positions)
{
    PosList directions = simplify(limitDirections(positions));
    double minLength = calcLength(directions) *minMatch;

    while(directions.size() > 0 && calcLength(removeShortestNoise(directions)) > minLength)
    {
        directions = simplify(removeShortestNoise(directions));
    }

    upNum = 0; downNum = 0; leftNum = 0; rightNum =0;
    for(int i = 0; i< directions.size(); i++)
    {
        if(directions[i].y >= 0 && directions[i].x ==0)
            upNum++;
        else if(directions[i].y < 0 && directions[i].x ==0)
            downNum++;
        else if(directions[i].x >= 0 && directions[i].y ==0 )
            leftNum++;
        else if(directions[i].x < 0 && directions[i].y ==0 )
            rightNum++;
    }
    return directions;
}


void GestureAlgorithm::updateStatistic(int x, int y)
{
    positions.push_back(Point(x, y));
    point_num = positions.size();
    if(point_num >1)
    {
        // For Point Recognization
        dist_sum += positions.begin()->dist(x,y);
        dist_average =dist_sum/(point_num - 1);

        // For Line Recognization
        // Need a patch for the V0 calculation.
        Point v0 = Point(positions[1].x - positions[0].x, positions[0].y );
        Point v1 = Point(x - positions[0].x, y -positions[0].y);
        if(normalize(v0) && normalize( v1))
        {
            float theta = acos(dot(v0, v1));
            theta_sum += theta;
            theta_sqsum += sq(theta);
            theta_average = theta_sum / (float)(point_num - 1);
            theta_factor = sqrt((float)(point_num - 1)*theta_sqsum - sq(theta_sum))/(point_num-1);
        }
    }
    mainDirections = detectDirection(positions);

    //Statistic Update
    pos_x_sum += x;
    pos_y_sum += y;
    pos_xx_sum += sq(x);
    pos_xy_sum += x * y;

    midPoint = Point(pos_x_sum/point_num, pos_y_sum/point_num);
    curGestureRender->render_bbox->addPoint(x, y);
}

bool GestureAlgorithm::calculateLine(float &a, float &b)
{

    float n = (float)positions.size();
    float div = n * pos_xx_sum - sq(pos_x_sum);
    if(div != 0)
    {
        float inv = 1.0f/div;
        a = (pos_y_sum * pos_xx_sum - pos_x_sum * pos_xy_sum) * inv;
        b = (n * pos_xy_sum - pos_x_sum * pos_y_sum) * inv;

        return true;
    }
    return false;
}


float GestureAlgorithm::getRadius()
{
    float maxr = std::numeric_limits<float>::min();
    for(int i = 0; i < positions.size(); i++)
    {
        float r = curGestureRender->render_bbox->getCenter().dist(positions[i].x, positions[i].y);
        maxr = max2(maxr, r);
    }
    return maxr;
}
