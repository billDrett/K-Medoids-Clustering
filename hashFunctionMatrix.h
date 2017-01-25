#ifndef Included_HashFunctionMatrix_H
#define Included_HashFunctionMatrix_H

#include "dataTypes.h"

class HashFunctionMatrix
{
    private:
        const int x1, x2;
        const double distX1X2;
        double median;

    public:
        HashFunctionMatrix(int X1, int X2, double DistX1X2);
        ~HashFunctionMatrix();

        bool value(MatrixPoint* key); //epistrefei thn timh ths h 0 'h 1
        double dCalculation(MatrixPoint* key);

        void setMedian(double medianValue);
};

double median(double* anArray, int no);
#endif

