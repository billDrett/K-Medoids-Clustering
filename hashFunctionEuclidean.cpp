#include <cmath>

#include "hashFunctionEuclidean.h"
#include "psedoRandomNumbers.h"

HashFunctionEuclidean::HashFunctionEuclidean(int dim, int W):dimensions(dim), w(W)
{
    vVariable = new double[dimensions];

    for(int i =0; i<dimensions; i++)
    {
        vVariable[i]=Marsaglia(); //tyxaia metavliti gauss
    }

    while((tVar = randomNumberDouble(0, w))== w){}; //einai anoixto diastima sto w, opote otan epistrefei w psaxnoume gia allo
}
HashFunctionEuclidean::~HashFunctionEuclidean()
{
    delete[] vVariable;
}

int HashFunctionEuclidean::value(Vector* key)
{
    double result=0;

    for(int i =0; i<dimensions;i++)
    {
        result += vVariable[i]*key->get_coordinance(i);
    }

    return (int)(floor((result+ tVar)/w));
}
