#ifndef Included_Lsh_H
#define Included_Lsh_H

#include <fstream>
#include "dataTypes.h"
#include "List.h"
#include "hashtable.h"
#include "clusterNode.h"
#include "distancesCalculations.h"

template<class T>
class LSH //class lsh
{
    private:
        const int L;
        const int K;

        HashTable<T>** hashtables; //pinakas apo hashtables
        List<T>* input; //eisodos
        Distance<T>* distances; //apostash

        bool elementExists(List<T>* tmpList, T key);
        double EuclideanNNTrick(T key, T& aproxNear);
        void initializeHashtables();

    public:
        LSH(List<T>* Input, int L1, int K1, Distance<T>* Distances);
        ~LSH();

        void runLSH(std::ofstream& outFile, List<T>* Search, double radius);
        void writeFile(std::ofstream& outFile, T queryPoint, List<T>* rangeNeigtbours, T aproxVector, double aproxDistance,  double trueDistance, double LSHTime, double bruteTime);
        void rangeSearch(T key, List<T>* result, double radius);
        void getAll(T key, List<T>* result); //returns all items which are in the same bucket as key

        double AproxNN(T key, T& aproxNear);
        double TrueNN(T key, T& aproxNear);

        void printSizeOfBuckets();
};

#endif
