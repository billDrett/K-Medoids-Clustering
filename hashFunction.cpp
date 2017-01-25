#include "hashFunction.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

//prototype gia thn hashFunction dn kanei kati
template <class T>
HashFunction<T>::HashFunction()
{

}

template <class T>
unsigned int HashFunction<T>::HashFunctionHash(T key) //prototype gia thn hashfunction
{

}

//---------------------------------------------------------

HashFunction<ClusterNode<Vector*>*>::HashFunction(int K, int Dimensions):kHashFunctions(K), dimensions(Dimensions)
{
    gFunction = new HashFunctionCosine*[kHashFunctions]();
    for(int i=0; i< kHashFunctions; i++)
    {
        gFunction[i] = new HashFunctionCosine(dimensions);
    }
}

HashFunction<ClusterNode<Vector*>*>::~HashFunction()
{
    for(int i =0; i < kHashFunctions; i++)
    {
        delete gFunction[i];
    }
    delete []gFunction;
}


unsigned int HashFunction<ClusterNode<Vector*>*>::HashFunctionHash(ClusterNode<Vector*>* key) //pairnoume thn timh ths g
{
    std::bitset<32> fBit; //mexri 2^32 buckets

    for(int i=0; i < kHashFunctions; i++) //gia kathe hi
    {
        if(gFunction[i]->value(key->getPoint())) //true
        {
            fBit[kHashFunctions-i-1] = 1; // h prwth h synartisi kathorizei to prwto bit tou bitset ara pairnoume to k-i-1 giati o pinakas ksekinaei apo k-1.
        }
        else
        {
            fBit[kHashFunctions-i-1] = 0;
        }
    }

    return fBit.to_ulong();
}

//---------------------------------------------

HashFunction<ClusterNode<Hamming*>*>::HashFunction(int K, int noBts):kHashFunctions(K), noBits(noBts)
{
    gFunction = new int[kHashFunctions];
    for(int i =0; i < kHashFunctions; i++)
    {
        gFunction[i] = randomNumberInt(0,noBts-1); //times apo 0 mexri number of bits-1
    }
}

HashFunction<ClusterNode<Hamming*>*>::~HashFunction()
{
    delete []gFunction;
}

unsigned int HashFunction<ClusterNode<Hamming*>*>::HashFunctionHash(ClusterNode<Hamming*>* key) //pairnoume thn timh ths g
{
    std::bitset<32> fBit; //2^32
    std::bitset<64> inputBitset; //h thesi 63 afora to pio simantiko bit kai h 0 to ligotero simantiko

    key->getPoint()->get_bitString(inputBitset);

    for(int i=0;i<kHashFunctions;i++) //gia kathe hi
    {
        fBit[kHashFunctions-i-1]=inputBitset[noBits-gFunction[i]-1]; //ksekiname apo to pio simantiko bit gia to prwto h.
    }

    return fBit.to_ulong();
}

//------------------------------------------------
HashFunction<ClusterNode<EuclideanNode*>*>::HashFunction(int K, int Dimensions, int W, int noBuckets):kHashFunctions(K), dimensions(Dimensions), nBuckets(noBuckets)
{
    gFunction = new HashFunctionEuclidean*[kHashFunctions]();
    for(int i=0; i< kHashFunctions; i++)
    {
        gFunction[i] = new HashFunctionEuclidean(dimensions, W);
    }

    rVariables = new int[kHashFunctions];
    for(int i=0; i< kHashFunctions;i++)
    {
        rVariables[i] = randomNumberInt(-32766, 32767);//enas tyxaios arithmos 16 bit
    }
}

HashFunction<ClusterNode<EuclideanNode*>*>::~HashFunction()
{
    for(int i =0; i < kHashFunctions; i++)
    {
        delete gFunction[i];
    }
    delete []gFunction;

    delete []rVariables;
}

unsigned int HashFunction<ClusterNode<EuclideanNode*>*>::HashFunctionHash(ClusterNode<EuclideanNode*>* key)
{
    return modulo(this->hashFunctionID(key), nBuckets);
}

int HashFunction<ClusterNode<EuclideanNode*>*>::hashFunctionID(ClusterNode<EuclideanNode*>* key)
{
    int result =0;

    for(int i=0; i < kHashFunctions; i++)
    {
        result += rVariables[i]*gFunction[i]->value(key->getPoint()->get_vector())%MAXINT; //eswteriko ginomeno mod M
    }

    return result%MAXINT;
}
//--------------------------------------------------------------------------------

HashFunction<ClusterNode<MatrixPoint*>*>::HashFunction(int K, List<ClusterNode<MatrixPoint*>*>* myInput): kHashFunctions(K)
{
    int x1, x2;
    double distx1x2, Median;
    ClusterNode<MatrixPoint*>* x1M = NULL;
    ClusterNode<MatrixPoint*>* x2M = NULL;
    double* myArray = new double[myInput->get_begin()->get_data()->getPoint()->get_noItems()];

    gFunction = new HashFunctionMatrix*[kHashFunctions]();
    for(int i=0; i< kHashFunctions; i++) //dimiourgoume ta k hi
    {
        x1 = randomNumberInt(0, myInput->get_begin()->get_data()->getPoint()->get_noItems()-1); //ena tyxaio shmeio
        while((x2 = randomNumberInt(0, myInput->get_begin()->get_data()->getPoint()->get_noItems()-1))==x1);  //oxi ta idia shmeia giati h metaksy tous apostash pou einai mhden mpainei ston paranomasth

        x1M = getPoint(myInput, x1); //vriskoume to random shmeio
        x2M = getPoint(myInput, x2);

        distx1x2 = x1M->getPoint()->get_distance(x2); //ypologisoume thn apostash metaksy tous
        gFunction[i] = new HashFunctionMatrix(x1, x2, distx1x2);

        int k=0;
        for(Node<ClusterNode<MatrixPoint*>*>* j = myInput->get_begin(); j != NULL; j = j->get_next())//ypologizw ola thn timh tou h gia kathe x
        {
            myArray[k] = gFunction[i]->dCalculation(j->get_data()->getPoint());
            k++;
        }

        Median = median(myArray, myInput->get_begin()->get_data()->getPoint()->get_noItems()); //vriskoume thn median
        gFunction[i]->setMedian(Median);
    }

    delete[] myArray;

}

HashFunction<ClusterNode<MatrixPoint*>*>::~HashFunction()
{
    for(int i =0; i < kHashFunctions; i++)
    {
        delete gFunction[i];
    }
    delete []gFunction;
}

unsigned int HashFunction<ClusterNode<MatrixPoint*>*>::HashFunctionHash(ClusterNode<MatrixPoint*>* key) //epistrefei thn g
{
    std::bitset<32> fBit; //2^32

    for(int i=0; i < kHashFunctions; i++)
    {
        if(gFunction[i]->value(key->getPoint())) //true
        {
            fBit[kHashFunctions-i-1] = 1;
        }
        else
        {
            fBit[kHashFunctions-i-1] = 0;
        }
    }
    return fBit.to_ulong();
}

ClusterNode<MatrixPoint*>* HashFunction<ClusterNode<MatrixPoint*>*>::getPoint(List<ClusterNode<MatrixPoint*>*>* myList, int pos) //epistrefei to tyxaio simeio
{
    for(Node<ClusterNode<MatrixPoint*>*>* i = myList->get_begin(); i != NULL; i = i->get_next())
    {
        if(pos == i->get_data()->getPoint()->get_pos())
        {
            return i->get_data();
        }
    }
    return NULL;
}

unsigned int modulo(int x1, int x2)
{
    return (x1%x2+x2)%x2;
}

template class HashFunction<ClusterNode<Vector*>*>;
template class HashFunction<ClusterNode<Hamming*>*>;
template class HashFunction<ClusterNode<EuclideanNode*>*>;
template class HashFunction<ClusterNode<MatrixPoint*>*>;
