#ifndef Included_HashFunctionEuclidean_H
#define Included_HashFunctionEuclidean_H

#include "dataTypes.h"

class HashFunctionEuclidean
{
    private:
        const int w;
        const int dimensions;
        double tVar;
        double* vVariable; //v kanoniki katanomh


    public:
        HashFunctionEuclidean(int dim, int W);
        ~HashFunctionEuclidean();

        int value(Vector* key); //epistrefei thn timh ths h
};
#endif

