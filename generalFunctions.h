#ifndef Included_GENERALFUNCTIONS_H
#define Included_GENERALFUNCTIONS_H

#include <iostream>

double maxDistance(double* distances, int size); //upologismos ths megisths apostashs se enan pinaka
void bubleSort(double* t, int no); //ta3inomhsh bubble sort

template <class T>
struct NodeDistance
{
    double dist;
    T item;
};

template <class T>
void quickSort(NodeDistance<T>* anArray, int start, int finish);

template <class T>
int partisionRandom(NodeDistance<T>* anArray, int start, int finish);

enum TYPE
{
    MAX,
    MIN
};

template <class T>
void sortPartOfArray(NodeDistance<T>* anArray, int size, int SortedItems, TYPE type);

int convertToBinary(char* bitString, int stringLength);
std::string convertToString(int number);

#endif
