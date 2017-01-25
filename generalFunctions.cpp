#include "generalFunctions.h"
#include "psedoRandomNumbers.h"
#include "euclideanNode.h"
#include "dataTypes.h"
#include "clusterNode.h"

#include <sstream>
#include <limits>

using namespace std;

void bubleSort(double* t, int no)
{
    double temp;
    for(int i=0;i<no;i++) //taksinomoume ton pinaka
    {
        for(int j=0;j<no-i;j++)
        {
            if(j!=(no-1)&&t[j]>t[j+1])
            {
                temp=t[j];
                t[j]=t[j+1];
                t[j+1]=temp;
            }
        }
    }
}

/*void quickSort(double* anArray, int start, int finish)
{
    int middle;
    if(start<finish)
    {
        middle = partisionRandom(anArray, start, finish);
        quickSort(anArray, start, middle-1);
        quickSort(anArray, middle+1, finish);
    }
}

int partisionRandom(double* anArray, int start, int finish)
{
    double val, temp;
    int i, q;

    q = start+ randomNumberInt(0, finish-start);
    val = anArray[q];
    anArray[q] = anArray[finish]; //antikatastoume to stoixeio q me to teleutaio
    anArray[finish] = val; //kai vazoume to q sto telos

    i = start-1;
    for(int j = start; j <finish; j++)
    {
        if(anArray[j]<=val)
        {
            i++;
            temp = anArray[i];
            anArray[i] = anArray[j];
            anArray[j] = temp;
        }
    }

    temp = anArray[i+1];
    anArray[i+1] = anArray[finish];
    anArray[finish] = temp;
    return i+1;
}
*/
template <class T>
void quickSort(NodeDistance<T>* anArray, int start, int finish)
{
    int middle;
    if(start<finish)
    {
        middle = partisionRandom(anArray, start, finish);
        quickSort(anArray, start, middle-1);
        quickSort(anArray, middle+1, finish);
    }
}

template <class T>
int partisionRandom(NodeDistance<T>* anArray, int start, int finish)
{
    NodeDistance<T> val, temp;
    int i, q;

    q = start+ randomNumberInt(0, finish-start);
    val = anArray[q];
    anArray[q] = anArray[finish]; //antikatastoume to stoixeio q me to teleutaio
    anArray[finish] = val; //kai vazoume to q sto telos

    i = start-1;
    for(int j = start; j <finish; j++)
    {
        if(anArray[j].dist<=val.dist)
        {
            i++;
            temp = anArray[i];
            anArray[i] = anArray[j];
            anArray[j] = temp;
        }
    }

    temp = anArray[i+1];
    anArray[i+1] = anArray[finish];
    anArray[finish] = temp;
    return i+1;
}

template <class T>
void sortPartOfArray(NodeDistance<T>* anArray, int size, int SortedItems, TYPE type) //SortedItems has to be small, otherwise call quickSort
{//similar to selection Sort
    NodeDistance<T> maxDist, minDist, temp;
    int pos;
    for(int i = 0; i < SortedItems; i++)
    {
        maxDist.dist = -std::numeric_limits<double>::min();
        minDist.dist = std::numeric_limits<double>::max();

        for(int j = i; j <size; j++)
        {
            if(type == MAX)
            {
                if(anArray[j].dist > maxDist.dist)
                {
                    maxDist = anArray[j];
                    pos = j;
                }
            }
            else
            {
                if(anArray[j].dist < minDist.dist)
                {
                    minDist = anArray[j];
                    pos = j;
                }
            }
        }
        //cout<<"max "<<maxDist.dist<<endl;
        if(type == MAX)
        {
            temp = anArray[i];
            anArray[i] = maxDist;
            anArray[pos] = temp;
        }
        else
        {
            temp = anArray[i];
            anArray[i] = minDist;
            anArray[pos] = temp;
        }
    }
}

double maxDistance(double* distances, int size)
{
    double maxDist = -std::numeric_limits<double>::min();

    for(int i =0; i < size; i++)
    {
        if(distances[i] > maxDist)
        {
            maxDist = distances[i];
        }
    }

    return maxDist;
}

int convertToBinary(char* bitString, int stringLength)
{
    int number = 0;
    int twoPower = 1;

    for(int i = stringLength-1; i >= 0; i--)
    {
        number += bitString[i]*twoPower;//(bitString[i]-'0')*twoPower; //we have the askii value of char 1 and 0, to get the value 1 or 0 we get the minus the askii value of '0'
        twoPower *=2;
    }

    return number;
}

std::string convertToString(int number)
{
    std::stringstream s; //make int to string
    s << number;

    return s.str();
}

//template int partisionRandom(NodeDistance<ClusterNode<MatrixPoint*>*>* anArray, int start, int finish);


template void quickSort(NodeDistance<ClusterNode<MatrixPoint*>*>* anArray, int start, int finish);
template void quickSort(NodeDistance<ClusterNode<Hamming*>*>* anArray, int start, int finish);
template void quickSort(NodeDistance<ClusterNode<Vector*>*>* anArray, int start, int finish);
template void quickSort(NodeDistance<ClusterNode<EuclideanNode*>*>* anArray, int start, int finish);
template void quickSort(NodeDistance<int>* anArray, int start, int finish);
template void quickSort(NodeDistance<char>* anArray, int start, int finish);


template void sortPartOfArray(NodeDistance<int>* anArray, int size, int SortedItems, TYPE type);
template void sortPartOfArray(NodeDistance<ClusterNode<Hamming*>*>* anArray, int size, int SortedItems, TYPE type);
template void sortPartOfArray(NodeDistance<ClusterNode<EuclideanNode*>*>* anArray, int size, int SortedItems, TYPE type);
template void sortPartOfArray(NodeDistance<ClusterNode<Vector*>*>* anArray, int size, int SortedItems, TYPE type);

