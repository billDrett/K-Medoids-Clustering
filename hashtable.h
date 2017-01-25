#ifndef Included_HashTable_H
#define Included_HashTable_H

#include "List.h"
#include "Node.h"
#include "hashFunction.h"

template <class T>
class HashTable //class Hashtable
{
    private:
        int nBuckets; //ari8mos twn buckets
        int totalSize; //sunoliko mege8os tou hashtable
        List<T>** buckets; //lista apo buckets 
        HashFunction<T>* gFunction; //gfunction

        unsigned int hashFunction(T key); //hash function key
 
    public:
        HashTable(int NBuckets, HashFunction<T>* hashMethod); //constructor
        ~HashTable(); //destructor

        int get_TotalSize(); //epistrofh total size
        int get_nBuckets(); //epistrofh ari8mou buckets
        HashFunction<T>* getHashFunction(); //epistrofh ths gFunction 

        Node<T>* get_bucket(T key); //epistregei thn lista pou antistixei se auto to bucket
        List<T>* get_bucketList(T key); //epistrofh tou deikth ths listas twn buckets pou anhkei to shmeio 

        void insertNode(T data); //eisagwgh node 

        bool checkEmpty(); //check an einai adeio to hashtable
        void printSizeOfBuckets(); //ektupwsh tou mege8ous twn buckets 
};

#endif

