#include "clusterNode.h"
#include "dataTypes.h"
#include "euclideanNode.h"

//kwdikas twn sunathsewn ths class ClusterNode

template <class T>
ClusterNode<T>::ClusterNode(T data)
{
    id = -1; //initialized in distance calculation for use in distance matrix
    point = data;

    secondCentroid = 0;  //dn exoun arxikes times
    distance = -1;
    secDistance = -1;
    checked = false;
}

template <class T>
ClusterNode<T>::ClusterNode(T data, double dist)
{
    id = -1; //initialized in distance calculation for use in distance matrixs
    point = data;
    distance = dist;

    secondCentroid = 0; //dn exoun arxikes times
    secDistance = -1;
    checked = false;
}

template <class T>
ClusterNode<T>::~ClusterNode()
{

}

template <class T>
int ClusterNode<T>::getID()
{
    return id;
}

template <class T>
T ClusterNode<T>::getPoint()
{
    return point;
}

template <class T>
double ClusterNode<T>::getDistance()
{
    return distance;
}

template <class T>
double ClusterNode<T>::getSecDistance()
{
    return secDistance;
}

template <class T>
ClusterNode<T>* ClusterNode<T>::getSecCentroid()
{
    return secondCentroid;
}

template <class T>
bool ClusterNode<T>::getChecked()
{
    return checked;
}

template <class T>
void ClusterNode<T>::setID(int ID)
{
    id = ID;
}

template <class T>
void ClusterNode<T>::setDistance(double dist)
{
    distance = dist;
}

template <class T>
void ClusterNode<T>::setSecDistance(double secDist)
{
    secDistance = secDist;
}

template <class T>
void ClusterNode<T>::setSecCentroid(ClusterNode<T>* centroid2)
{
    secondCentroid = centroid2;
}

template <class T>
void ClusterNode<T>::setChecked(bool check)
{
    checked = check;
}

template class ClusterNode<Vector* >;
template class ClusterNode<Hamming* >;
template class ClusterNode<EuclideanNode* >;
template class ClusterNode<MatrixPoint* >;
