#include <math.h>
#include <limits>
#include "lsh.h"
#include <ctime>

using namespace std;

//prototypo gia ton constructor ths LSH, ton eidikeuoume gia kathe periptwsh
template <class T>
LSH<T>::LSH(List<T>* Input,  int L1, int K1, Distance<T>* Distances):L(L1), K(K1)
{
    hashtables = NULL;
}


template <>
LSH<ClusterNode<Hamming*>*>::LSH(List<ClusterNode<Hamming*>*>* Input, int L1, int K1, Distance<ClusterNode<Hamming*>*>* Distances): L(L1), K(K1)
{
    const int noBuckets = pow(2, K);
    const int noBits = Input->get_begin()->get_data()->getPoint()->get_noBits();
    HashFunction<ClusterNode<Hamming*>*>* hashFunct;

    input = Input;

    hashtables = new HashTable<ClusterNode<Hamming*>*>*[L]; //kataskeuazoume ena pinaka apo listes
    for(int i = 0; i < L; i++) //kataskeuazoume L hashtable
    {
        hashFunct = new HashFunction<ClusterNode<Hamming*>*>(K, noBits); //thn hamming hash function
        hashtables[i] = new HashTable<ClusterNode<Hamming*>*>(noBuckets, hashFunct);
    }

    initializeHashtables();
    distances = Distances;
}

template <>
LSH<ClusterNode<Vector*>*>::LSH(List<ClusterNode<Vector*>*>* Input, int L1, int K1, Distance<ClusterNode<Vector*>*>* Distances):L(L1), K(K1)
{
    const int noBuckets = pow(2, K);
    const int dimensions = Input->get_begin()->get_data()->getPoint()->get_dimensions();
    HashFunction<ClusterNode<Vector*>*>* hashFunct;

    input = Input;

    hashtables = new HashTable<ClusterNode<Vector*>*>*[L]; //kataskeuazoume ena pinaka apo listes
    for(int i = 0; i < L; i++) //kataskeuazoume L hashtable
    {
        hashFunct = new HashFunction<ClusterNode<Vector*>*>(K, dimensions); //thn cosine hash function
        hashtables[i] = new HashTable<ClusterNode<Vector*>*>(noBuckets, hashFunct);
    }

    initializeHashtables();
    distances = Distances;

}
/*
template <>
void LSH<EuclideanNode*>::initializeHashtables()
{
    Node<EuclideanNode*>* node = input->get_begin();
    EuclideanNode* tmpNode;
    int sizeList = input->getSize();
    for(int i =0; i <sizeList; i++) //vazoyme kathe stoixeio se ola ta hashtable pou ftiaksame
    {
        for(int j = 0; j < L; j++)
        {
            tmpNode = new EuclideanNode(node->get_data()->get_vector()); //ftiaxnw neo stoixeio giati se kathe table tha exoun diaforetiko ID
            input->insertEnd(tmpNode); //to vazw se lista gia diagrafh argotera

            hashtables[j]->insertNode(tmpNode);
        }
        node=node->get_next();
    }
}*/

template <>
LSH<ClusterNode<EuclideanNode*>*>::LSH(List<ClusterNode<EuclideanNode*>*>* Input, int L1, int K1, Distance<ClusterNode<EuclideanNode*>*>* Distances):L(L1), K(K1)
{
    const int noBuckets = Input->getSize()/32; // n/8, opou n to synolo twn stoixeiwn
    const int dimensions = Input->get_begin()->get_data()->getPoint()->get_vector()->get_dimensions();
    HashFunction<ClusterNode<EuclideanNode*>*>* hashFunct;

    input = Input;

    hashtables = new HashTable<ClusterNode<EuclideanNode*>*>*[L]; //kataskeuazoume ena pinaka apo listes
    for(int i = 0; i < L; i++) //kataskeuazoume L hashtable
    {
        hashFunct = new HashFunction<ClusterNode<EuclideanNode*>*>(K, dimensions, 4, noBuckets); //thn eukleidia hash function
        hashtables[i] = new HashTable<ClusterNode<EuclideanNode*>*>(noBuckets, hashFunct);
    }

    /*Node<EuclideanNode*>* node = input->get_begin();
    EuclideanNode* tmpNode;
    int sizeList = input->getSize();
    for(int i =0; i <sizeList; i++) //vazoyme kathe stoixeio se ola ta hashtable pou ftiaksame
    {
        for(int j = 0; j < L; j++)
        {
            tmpNode = new EuclideanNode(node->get_data()->get_vector()); //ftiaxnw neo stoixeio giati se kathe table tha exoun diaforetiko ID
            input->insertEnd(tmpNode); //to vazw se lista gia diagrafh argotera

            hashtables[j]->insertNode(tmpNode);
        }
        node=node->get_next();
    }*/

    initializeHashtables();
    distances = Distances;
}

template <>
LSH<ClusterNode<MatrixPoint*>*>::LSH(List<ClusterNode<MatrixPoint*>*>* Input, int L1, int K1, Distance<ClusterNode<MatrixPoint*>*>* Distances):L(L1), K(K1)
{
    const int noBuckets = pow(2, K);
    HashFunction<ClusterNode<MatrixPoint*>*>* hashFunct;

    input = Input;

    hashtables = new HashTable<ClusterNode<MatrixPoint*>*>*[L]; //kataskeuazoume ena pinaka apo listes
    for(int i = 0; i < L; i++) //kataskeuazoume L hashtable
    {
        hashFunct = new HashFunction<ClusterNode<MatrixPoint*>*>(K, input); //thn matrix hash function
        hashtables[i] = new HashTable<ClusterNode<MatrixPoint*>*>(noBuckets, hashFunct);
    }

    initializeHashtables();
    distances = Distances;
}

template <class T>
LSH<T>::~LSH()
{
    if(hashtables == NULL) return;

    for(int i =0; i < L; i++)
    {
        delete hashtables[i]->getHashFunction();
        delete hashtables[i];
    }
    delete []hashtables;
}

template<class T>
void LSH<T>::runLSH(std::ofstream& outFile, List<T>* Search, double radius)
{
    double aproxDistance, trueDistance;
    T aproxNear;
    T trueNear;
    List<T>* result= new List<T>();
    clock_t begin, end;
    double LSHTime, BruteTime;

    for(Node<T>* i = Search->get_begin(); i != NULL; i = i->get_next()) //gia kathe simeio sto query list
    {
        rangeSearch(i->get_data(), result, radius); //anazhthsh R geitonwn
        begin = clock();
        aproxDistance = AproxNN(i->get_data(), aproxNear);  //aproximate nearest neighbor
        end = clock();
        LSHTime = (double)(end-begin)/CLOCKS_PER_SEC; //ypologizw ton xrono
        begin = clock();
        trueDistance = TrueNN(i->get_data(), trueNear); //pragmatika kontinoteros
        end = clock();
        BruteTime = (double)(end-begin)/CLOCKS_PER_SEC;

        //grafoume ta apotelesmata sto arxeio
        writeFile(outFile, i->get_data(), result, aproxNear, aproxDistance, trueDistance, LSHTime, BruteTime);

        while(result->deleteFirstNode() != NULL) //adeiazw thn lista gia to epomeno query point
        {
        }
    }

    delete result;
}


template<class T>
double LSH<T>::EuclideanNNTrick(T key, T& aproxNear) //mono prototypo gia thn eukleidia
{
    return-1;
}
/*
template<>
double LSH<EuclideanNode*>::EuclideanNNTrick(EuclideanNode* key, EuclideanNode*& aproxNear)
{
    EuclideanNode* minElement = NULL;
    double minDistance = std::numeric_limits<double>::max();
    double tmpDistance;

    for(int i = 0; i < L; i++)
    {
        for(Node<EuclideanNode*>* j = hashtables[i]->get_bucket(key); j != NULL; j =j->get_next())
        {
            if(j->get_data() == NULL) continue; //barrier
            if(j->get_data()->get_ID() != key->get_ID()) continue;

            tmpDistance = distance(j->get_data(), key);
            if(tmpDistance < minDistance)
            {
                minElement = j->get_data();
                minDistance = tmpDistance;
            }
        }
    }

    aproxNear= minElement;
    return minDistance;
}*/

//allages gia thn periptwsh ths eukleidias
/*template<>
void LSH<EuclideanNode*>::runLSH(std::ofstream& outFile, List<EuclideanNode*>* Search, double radius)
{
    double aproxDistance, trueDistance;
    EuclideanNode* aproxNear = NULL;
    EuclideanNode* trueNear = NULL;
    List<EuclideanNode*>* result= new List<EuclideanNode*>();
    clock_t begin, end;
    double LSHTime, BruteTime;


    for(Node<EuclideanNode*>* i = Search->get_begin(); i != NULL; i = i->get_next())
    {
        rangeSearch(i->get_data(), result, radius); //anazhthsh R geitonwn

        begin = clock();
        //exw ylopoihsei to trick alla merikes fores dn vriskei ta kalytera apotelesmata opote to vazw san sxolio

        /*aproxDistance = EuclideanNNTrick(i->get_data(), aproxNear);
        if(aproxNear == NULL) //dn vrikame kati me to trick
        {
            aproxDistance = AproxNN(i->get_data(), aproxNear);
        }*/
  /*      aproxDistance = AproxNN(i->get_data(), aproxNear); //ypologizw ton aproximate NN
        end = clock();

        LSHTime = (double)(end-begin)/CLOCKS_PER_SEC;
        begin = clock();
        trueDistance = TrueNN(i->get_data(), trueNear); // ypologizw ton pragrmatika kontinotero
        end = clock();
        BruteTime = (double)(end-begin)/CLOCKS_PER_SEC;

        //grapsimo se arxeio
        writeFile(outFile, i->get_data(), result, aproxNear, aproxDistance, trueDistance, LSHTime, BruteTime);


        while(result->deleteFirstNode() != NULL) //adeiazw thn lista gia to epomeno query point
        {
        }

    }

    delete result;
}*/

template<class T>
void LSH<T>::writeFile(std::ofstream& outFile, T queryPoint, List<T>* rangeNeigtbours, T aproxVector, double aproxDistance,  double trueDistance, double LSHTime, double bruteTime)
{//grafei sto arxeio thn eksodo tou query point
    outFile << "Query: " << queryPoint->getPoint()->get_string()<<endl;
    outFile << "R-near neighbors:"<<endl;
    for(Node<T>* i = rangeNeigtbours->get_begin(); i != NULL; i = i->get_next())
    {
        outFile<<i->get_data()->getPoint()->get_string()<<endl;
    }

    if(aproxDistance == std::numeric_limits<double>::max()) //dn vrethike
    {
        outFile << "Nearest neighbor: not found"<<endl;
        outFile << "DistanceLSH: not found"<<endl;
    }
    else
    {
        outFile << "Nearest neighbor: "<<aproxVector->getPoint()->get_string()<<endl;
        outFile << "DistanceLSH: "<<aproxDistance<<endl;
    }

    outFile << "DistanceTrue: "<<trueDistance<<endl;
    outFile << "tLSH: "<< LSHTime<<endl;
    outFile << "tTrue: " << bruteTime<<endl;
    outFile<<endl;
}


template <class T>
void LSH<T>::getAll(T key, List<T>* result)//void LSH<T>::rangeSearchBarrier(T key, List<T>* result, double radius)
{
    List<T>* tmpList;
    T data;
    Node<T>* tmp;
    double dist;

    for(int i = 0; i < L; i++)
    {
        tmpList = hashtables[i]->get_bucketList(key);

        for(Node<T>* j = hashtables[i]->get_bucket(key); j != NULL; j=j->get_next())
        {
            if(j->get_data()->getPoint()->get_string() == key->getPoint()->get_string()) continue; //an einai to idio to agnow
            if(elementExists(result, j->get_data())) continue; //yparxei hdh ara dn to vazw sthn lista

            result->insertEnd(j->get_data());
        }

    }
}


template <class T>
void LSH<T>::rangeSearch(T key, List<T>* result, double radius)//void LSH<T>::rangeSearchBarrier(T key, List<T>* result, double radius)
{
    List<T>* tmpList;
    T data;
    Node<T>* tmp;
    double dist;

    for(int i = 0; i < L; i++)
    {
        tmpList = hashtables[i]->get_bucketList(key);

        for(Node<T>* j = hashtables[i]->get_bucket(key); j != NULL; j=j->get_next())
        {
            if(j->get_data()->getPoint()->get_string() == key->getPoint()->get_string()) continue; //an einai to idio to agnow
            if(j->get_data()->getChecked()) //an exei elenxthei tote dn to ksanaelenxw
            {
                continue;
            }

            if(distances->distance(j->get_data(), key) < radius)
            {
                if(elementExists(result, j->get_data())) continue; //yparxei hdh ara dn to vazw sthn lista
                result->insertEnd(j->get_data());
            }
        }

    }
}

template <class T>
double LSH<T>::TrueNN(T key, T& aproxNear) //brute force gia pragmatika kontinotero
{
    double minDistance = std::numeric_limits<double>::max(); //dinoume thn megalyterh timh pou mporei na parei o double
    double tmpDistance;

    for(Node<T>*i = input->get_begin(); i != NULL; i = i->get_next())
    {
        tmpDistance = distances->distance(i->get_data(), key);

        if(tmpDistance < minDistance)
        {
            minDistance = tmpDistance;
            aproxNear = i->get_data();
        }
    }

    return minDistance;
}

template <class T>
double LSH<T>::AproxNN(T key, T& aproxNear) //epistrefei ton kontinotero symfwna me to lsh
{
    T minElement = 0;
    double minDistance = std::numeric_limits<double>::max();
    double tmpDistance;
    int totalItems = 0;

    for(int i = 0; i < L; i++)
    {
        for(Node<T>* j = hashtables[i]->get_bucket(key); j != NULL; j =j->get_next())
        {
            if(j->get_data() == NULL) continue; //barrier
           /* if(totalItems > 3*L) //orio gia to plithos twn elenxwn
            {
                totalItems++;
                aproxNear = minElement;
                delete tmpList;
                return minDistance;
            }*/

            tmpDistance = distances->distance(j->get_data(), key);
            if(tmpDistance < minDistance)
            {
                minElement = j->get_data();
                minDistance = tmpDistance;
            }
        }
    }

    aproxNear= minElement;
    return minDistance;
}

template <class T>
void LSH<T>::initializeHashtables()
{
    for(Node<T>* i = input->get_begin(); i != NULL; i = i->get_next()) //vazoyme kathe stoixeio se ola ta hashtable pou ftiaksame
    {
        for(int j = 0; j < L; j++)
        {
            hashtables[j]->insertNode(i->get_data());
        }
    }
}

template <class T>
bool LSH<T>::elementExists(List<T>* tmpList, T key) //elenxei an yparxei hdh to stoixeio sthn lista
{
    for(Node<T>* i = tmpList->get_begin(); i!= NULL; i = i->get_next()) //pairnw ta stoixeia pou exoun elengthei
    {
        if(i->get_data() == key)//if(i->get_data()->getPoint()->get_string() == key->getPoint()->get_string())
        {
            return true;
        }
    }

    return false;
}

template <class T>
void LSH<T>::printSizeOfBuckets()
{
    hashtables[0]->printSizeOfBuckets();
}

template class LSH<ClusterNode<Vector*>*>;
template class LSH<ClusterNode<Hamming*>*>;
template class LSH<ClusterNode<EuclideanNode*>*>;
template class LSH<ClusterNode<MatrixPoint*>*>;
