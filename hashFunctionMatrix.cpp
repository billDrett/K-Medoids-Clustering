#include "hashFunctionMatrix.h"
#include "psedoRandomNumbers.h"
#include "generalFunctions.h"
#include <cmath>

HashFunctionMatrix::HashFunctionMatrix(int X1, int X2, double DistX1X2):x1(X1), x2(X2), distX1X2(DistX1X2)
{
}

HashFunctionMatrix::~HashFunctionMatrix()
{
}

bool HashFunctionMatrix::value(MatrixPoint* key)
{
    return (dCalculation(key) >= median);
}

double HashFunctionMatrix::dCalculation(MatrixPoint* key)
{
    return (pow(key->get_distance(x1), 2)+pow(key->get_distance(x2), 2) - pow(distX1X2, 2))/(2*distX1X2); //ypologizei thn timh ths h
}

void HashFunctionMatrix::setMedian(double medianValue)
{
    median = medianValue;
}

double median(double* anArray, int no)
{
    NodeDistance<char>* sortedArray = new NodeDistance<char>[no]; //create array for qicksort
    for(int i =0; i < no; i++)
    {
        sortedArray[i].dist = anArray[i];
    }
    quickSort(sortedArray, 0, no-1);

    if(no%2==0) //artios
    {
        return (sortedArray[no/2].dist+sortedArray[no/2-1].dist)/2; //pairnoume ta dyo mesaia kai vriskoume mesw oro
    }
    else //perittos
    {
        return sortedArray[(no/2)].dist; //pairnoume to mesaio
    }

    delete[]sortedArray;

}
