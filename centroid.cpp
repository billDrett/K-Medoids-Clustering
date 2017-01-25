#include "centroid.h"
#include <stdlib.h>
#include "dataTypes.h"
#include "euclideanNode.h"

//kwdikas twn sunarthswen ths class Cluster

template <class T>
Cluster<T>::Cluster()
{
    points = new List<ClusterNode<T>*>();
    centroid = NULL;
}

template <class T>
Cluster<T>::Cluster(ClusterNode<T>* centr)
{
    points = new List<ClusterNode<T>*>();
    centroid = centr;
}

template <class T>
Cluster<T>::~Cluster()
{
    
    delete points;
}

template <class T>
Node<ClusterNode<T>*>* Cluster<T>::getPoints()
{
    return points->get_begin();
}

template <class T>
int Cluster<T>::getNoPoints()
{
    return points->getSize();
}

template <class T>
ClusterNode<T>* Cluster<T>::getCentroid()
{
    return centroid;
}

template <class T>
void Cluster<T>::setCentroid(ClusterNode<T>* centr)
{
    centroid = centr;
}

template <class T>
void Cluster<T>::insertPoint(ClusterNode<T>* point)
{
    points->insertEnd(point);
}

template <class T>
void Cluster<T>::deletePoint(Node<ClusterNode<T>*>* delPoint)  //diagrafh tou shmeiou
{
    points->deleteNode(delPoint);
}

template <class T>
void Cluster<T>::emptyCluster()
{
    while(points->deleteFirstNode() != NULL){}
}

template class Cluster<Vector* >;
template class Cluster<Hamming* >;
template class Cluster<EuclideanNode* >;
template class Cluster<MatrixPoint* >;
