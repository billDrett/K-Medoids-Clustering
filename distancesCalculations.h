#ifndef Included_Distance_H
#define Included_Distance_H

#include "dataTypes.h"
#include "euclideanNode.h"
#include "clusterNode.h"
#include "List.h"

using namespace std;
#include <cmath>
#include <limits>

template <class T>
class Distance //klash gia ton upologismo apostashs duo items
{
    private:
        int noItems; //ari8mos twn items
        double** distances; //pinakas apostasewn

    public:
        Distance(List<T>* inputList);
      //  Distance(T* inArray, int arraySize);

        ~Distance(); //destructor

        double** getArray(); //epistrofh deikth sthn arxh tou pinaka
        static double distance(Vector* point1, Vector* point2); //upologismos apostashs gia cosine
        static int distance(Hamming* point1, Hamming* point2); //upologismos apostashs gia Hamming
        static double distance(EuclideanNode* point1, EuclideanNode* point2); //upologismos apostashs gia Euclidean
        static double distance(MatrixPoint* point1, MatrixPoint* point2); //upologismos apostashs gia MatrixPoint

        double distance(T point1, T point2); //epistrofh ths apostashs duo sugkekrimenwn shmeiwn
};

#endif
