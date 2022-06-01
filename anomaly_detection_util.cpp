//324680438
//313306367
/*************
* Nitzan Fisher & David Monheit
* ex2
* ************/
#include "anomaly_detection_util.h"
#include <cmath>
/******
* Function Name: simpleAvg
* Input: float* X - array of floats, float size - the array size.
* Output: float
* Function Operation: the function calculates the average of the numbers that in the array.
******/
float simpleAvg(float* x, int size){
    if(size <= 0){
        return 0;
    }
    float sum = 0;
    float u;
    //loop to calculate average
    for (int i = 0; i < size; i++) {
        sum += *x;
        x++;
    }
    u = sum / (float)size;
    return u;
}

/******
* Function Name: var
* Input: float* X - array of floats, int size - the array size.
* Output: float
* Function Operation: the function calculates the variance of the numbers that in the array,
 * using the simpleAvg function.
******/
float var(float* x, int size){
    if(size <= 0){
        return 0;
    }
    //set initial sum to 0
    float sum = 0;
    float u = simpleAvg(x, size);
    float variance;
    //calculate for every number in the array.
    for (int i = 0; i < size; i++) {
        float tmp = *x-u;
        sum += powf(tmp,2);
        x++;
    }
    variance = sum / (float)size;
    return variance;
}

/******
* Function Name: cov
* Input: float* X, float* Y - 2 arrays of floats, int size - the array size.
* Output: float
* Function Operation: the function calculates the covariance of the numbers that in the arrays,
 * using the simpleAvg and the var functions.
******/
float cov(float* x, float* y, int size){
    if(size <= 0){
        return 0;
    }
    float xTimesY[size];
    for (int i = 0; i < size; i++){
        xTimesY[i] = x[i] * y[i];
    }
    return simpleAvg(xTimesY, size) - (simpleAvg(x, size)* simpleAvg(y, size));
}

/******
* Function Name: pearson
* Input: float* X, float* Y - 2 arrays of floats, int size - the array size.
* Output: float
* Function Operation: the function calculates the pearson of the numbers that in the arrays,
 * using the cov and the var functions.
******/
float pearson(float* x, float* y, int size){
    float covXY = cov(x, y, size);
    float sqrtVarX = sqrtf(var(x, size));
    float sqrtVarY = sqrtf(var(y, size));

    return covXY / (sqrtVarX * sqrtVarY);
}

/******
* Function Name: linear_reg
* Input: Point** points - array of pointers to points, int size - the size of the array.
* Output: Line
* Function Operation: the function performs linear regression on the points array and returns the
 * line equation of the line.
******/
Line linear_reg(Point** points, int size){
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = (points[i])->x;
        y[i] = (points[i])->y;
    }

    //calculates the variance, covariance and then the average.
    float covXY = cov(x, y, size);
    float varX = var(x, size);
    float a = covXY/varX;
    float avgY = simpleAvg(y, size);
    float avgX = simpleAvg(x, size);
    //b = y - ax
    float b = avgY - (a * avgX);

    return {a,b};
}

/******
* Function Name: dev
* Input: Point p - point, Point** points - array of pointers to points, int size - the size of the array.
* Output: float
* Function Operation: the function returns the deviation between the point and the line
 * that made from the linear regression on the points array.
******/
float dev(Point p,Point** points, int size){
    Line linearReg = linear_reg(points, size);
    return dev(p, linearReg);
}

/******
* Function Name: dev
* Input: Point p - point, Line l - line.
* Output: float
* Function Operation: the function returns the deviation between the point and the line.
******/
float dev(Point p,Line l){
    float lineY = l.f(p.x);
    float yDiff = p.y - lineY;
    float distance = sqrtf(powf(yDiff, 2));
    return distance;
}

