#ifndef Included_DataTypes_H
#define Included_DataTypes_H

#include <iostream>
#include "stdio.h"
#include "string.h"
#include <bitset>
//orismos twn tupwn dedomenwn pou 8a apo8hkeuoume kai 8a diaxeirizomaste
class Vector //class Vector gia dedomena cosine
{
    private:
        std::string id;
        const int dimensions;
        double* coordinance;
    public:
        Vector(std::string ID, int dim, double* coord): dimensions(dim)
        {
            id = ID;
            coordinance = new double[dimensions];
            memcpy(coordinance, coord, dimensions*sizeof(double)); //antigrafei ta dedomena apo ton ena pinaka ston allo
        }
        ~Vector()
        {
            delete[] coordinance;
        }

        std::string get_string()
        {
            return id;
        }

        int get_dimensions()
        {
            return dimensions;
        }

        double get_coordinance(int pos)
        {
            if(pos >= dimensions) return -9999;
            return coordinance[pos];
        }
};

class Hamming //class Hamming gia dedomena tupou hamming
{
    private:
        std::string id;
        std::bitset<64> bitString;
         const int noBits;

    public:
        Hamming(std::string ID, std::bitset<64> bitSet, int nBits): noBits(nBits)
        {
            id = ID;
            bitString = bitSet;
        }

        std::string get_string()
        {
            return id;
        }

        void get_bitString(std::bitset<64>& data)
        {
            data = bitString;
        }

        int get_noBits()
        {
            return noBits;
        }
};

class MatrixPoint //class MatrixPoint gia dedomena pou anhkoun se distance matrix
{
    private:
        std::string id;
        const int N, position;
        int* distances;

    public:
        MatrixPoint(std::string ID, int* dist,  int n, int pos): N(n), position(pos)
        {
            id = ID;
            distances = new int[N];
            memcpy(distances, dist, N*sizeof(int));
        }

        ~MatrixPoint()
        {
            delete [] distances;
        }

        std::string get_string()
        {
            return id;
        }

        int get_noItems()
        {
            return N;
        }

        int get_distance(int pos)
        {
            if(pos >= N) return -9999;
            return distances[pos];
        }

        int get_pos()
        {
            return position;
        }
};
#endif

