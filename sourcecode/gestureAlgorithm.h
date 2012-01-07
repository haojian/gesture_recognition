#ifndef GESTUREALGORITHM_H
#define GESTUREALGORITHM_H

#include "gesture_DS.h"
#include "gestureRender.h"

class GestureAlgorithm
{
public:
    GestureAlgorithm( int minimumMovement = 5, double minimumMatch = 0.9 );
    ~GestureAlgorithm();

    void init();        // init the algorithm
    void reset();       // reset
    void startGesture( int x, int y );
    void addPoint( int x, int y );
    void endGesture( int x, int y );
    Render_Gesture *curGestureRender;

private:
    virtual void recognizeGesture();
    virtual void updateStatistic(int x, int y);
    float getRadius();

    PosList limitDirections( const PosList &positions );
    PosList simplify( const PosList &positions );
    PosList removeShortestNoise(const PosList &positions);
    PosList detectDirection(const PosList &positions);
    int calcLength( const PosList &positions );
    bool calculateLine(float &a, float &b);

    bool isStraightLine();
    bool isCircle();
    bool isEraze();
    bool isPoint();
    bool isPolyline();

    int minMovement2;
    double minMatch;
    float relativeIgnoreFactor;
    float absoluteIgnoreFactor;
    int totalLength;

    ////////////////////////////////////////////////////
    //recognization variables.
    ////////////////////////////////////////////////////
    vector<int> test, test1;

    PosList positions , mainDirections, breakPoints;
    //Point Judgement
    bool bIsPoint, bIsStraightLine, bIsShapeFixed;

    E_GESTURE_TYPE curGestureType, lastGestureType;
    int calculateIndex;

    ////////////////////////////////////////////////////
    //Statistic variables.
    ////////////////////////////////////////////////////
    float dist_sum, dist_average, theta_sum, theta_average, theta_sqsum, theta_factor;
    float pos_x_sum, pos_y_sum, pos_xx_sum, pos_xy_sum;
    int point_num;
    int upNum, downNum, leftNum, rightNum;
    Point midPoint;
};
#endif // GESTUREALGORITHM_H
