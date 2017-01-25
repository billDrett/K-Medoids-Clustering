#include "hashFunctionCosine.h"
#include "psedoRandomNumbers.h"

using namespace std;
#include <iostream>
HashFunctionCosine::HashFunctionCosine(int dim):dimensions(dim)
{
    rVariable = new double[dimensions];

    for(int i =0; i<dimensions; i++)
    {
        rVariable[i]=Marsaglia(); //tyxaia metavlhth gauss
    }
}

HashFunctionCosine::~HashFunctionCosine()
{
    delete[] rVariable;
}

bool HashFunctionCosine::value(Vector* key)
{
    double result=0;

    for(int i =0; i<dimensions;i++)//vriskw to eswteriko ginomeno
    {
        result += rVariable[i]*key->get_coordinance(i);
    }

    return (result >= 0); //an einai thetiko to bit einai 1
}
