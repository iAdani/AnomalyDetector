// Author: Guy Adani 208642884

#include <math.h>
#include "anomaly_detection_util.h"
#include <iostream>

// returns a simple average
float avg(float* x, int size){
	float sum = 0;

    for(int i = 0; i < size; i++) {
        sum += x[i];
    }

    return (sum / size);
}

// returns the variance of X and Y
float var(float* x, int size){
	float miu = 0; // "tohelet"
    float varx = 0; // variance of x

    // calculate the Sigma's
    for(int i = 0; i < size; i++) {
        miu += x[i];
        varx += (x[i] * x[i]);
    }
    miu /= size;
    varx /= size;

    // calculate and return Var(x)
    varx -= (miu * miu);
    return varx;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	// calculate E(x) and E(y)
    float eX = avg(x, size);
    float eY = avg(y, size);

    // calculate and return cov(x,y)
    float covXY = 0;
    for(int i = 0; i < size; i++) {
        covXY += (x[i] - eX) * (y[i] - eY);
    }
    return covXY / size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	float covXY = cov(x,y,size);
    float sigmaX = sqrt(var(x, size));
    float sigmaY = sqrt(var(y, size));

    return covXY / (sigmaX * sigmaY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
// separate the x and y of the points to arrays
    float arrX [size];
    float arrY [size];
    for (int i = 0; i < size; i++) {
        arrX[i] = points[i]->x;
        arrY[i] = points[i]->y;
    }

    // calculate a
    float a = cov(arrX, arrY, size) / var(arrX, size);

    // calculate averages X and Y
    float b = 0, aveX = 0, aveY = 0;
    aveX = avg(arrX, size);
    aveY = avg(arrY, size);
    
    // delete[] arrX;
    // delete[] arrY;

    // calculate b
    b = aveY - (a * aveX);

    return Line (a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	// make a line from the points
    Line line = linear_reg(points, size);

    // return the deviation using the other dev function
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	// calculate f(x)
    float fx = l.a * p.x + l.b;

    // return |f(x) - y|
    float dev = fx - p.y;
    if (dev < 0) {
        return -dev;
    }
    return dev;
}