#include <cstdlib>
#include <iostream>

#include "hashtable.h"
#include "hashFunction.h"
#include "euclideanNode.h"

using namespace std;

template <class T>
HashTable<T>::HashTable(int NBuckets, HashFunction<T>* hashMethod)
{
    if(NBuckets <= 0) return;

    nBuckets = NBuckets;
    totalSize = 0;

    buckets = new List<T> *[nBuckets]; //kataskeuazoume ena pinaka apo listes
    for(int i = 0; i < nBuckets; i++)
    {
        buckets[i] = new List<T>();
    }

    gFunction = hashMethod; //orizoume thn hash function
}

template <class T>
HashTable<T>::~HashTable()
{
    for(int i = 0; i < nBuckets; i++)
    {
        delete buckets[i];
    }
    delete []buckets;
}

template <class T>
unsigned int HashTable<T>::hashFunction(T key) //epistrefei thn timh ths hash function
{
    return gFunction->HashFunctionHash(key);
}

template <class T>
int HashTable<T>::get_TotalSize()
{
    return totalSize;
}

template <class T>
int HashTable<T>::get_nBuckets()
{
    return nBuckets;
}

template <class T>
HashFunction<T>* HashTable<T>::getHashFunction()
{
    return gFunction;
}

template <class T>
Node<T>* HashTable<T>::get_bucket(T key) //epistregei thn lista pou antistixei se auto to bucket
{
    return buckets[hashFunction(key)]->get_begin();
}

template <class T>
List<T>* HashTable<T>::get_bucketList(T key)
{
    return buckets[hashFunction(key)];
}

template <class T>
void HashTable<T>::insertNode(T data)
{
    totalSize++;
    buckets[hashFunction(data)]->insertEnd(data); //vazei to stoixeio sthn thesi pou leei to hashfunction

}

template <class T>
bool HashTable<T>::checkEmpty() //elenxei an einai keno to hashtable
{
    for(int i=0;i< nBuckets;i++)
    {
        if(buckets[i]->checkEmpty())
        {
            return true;
        }
    }

    return false;
}


template <class T>
void HashTable<T>::printSizeOfBuckets() //emfanizei to megethos tou kathe bucket
{
    for(int i =0; i < nBuckets; i++)
    {
        cout<<i<<"     "<<buckets[i]->getSize()<<endl;
    }
}

template class HashTable<ClusterNode<Vector*>*>;
template class HashTable<ClusterNode<Hamming*>*>;
template class HashTable<ClusterNode<EuclideanNode*>*>;
template class HashTable<ClusterNode<MatrixPoint*>*>;

