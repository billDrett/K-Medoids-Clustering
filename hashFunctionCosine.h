#ifndef Included_HashFunctionCosine_H
#define Included_HashFunctionCosine_H

#include "dataTypes.h"

class HashFunctionCosine
{
    private:
        const int dimensions;
        double* rVariable; //r kanoniki katanomh

    public:
        HashFunctionCosine(int dim);
        ~HashFunctionCosine();

        bool value(Vector* key); //epistrefei thn timh ths h 0 'h 1
};
#endif

