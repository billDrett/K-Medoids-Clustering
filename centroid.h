#ifndef Included_Cluster_H
#define Included_Cluster_H

#include "List.h"
#include "Node.h"
#include "clusterNode.h"

template <class T>
class Cluster
{
    private:
        ClusterNode<T>* centroid; //kentroeides
        List<ClusterNode<T>*>* points; //lista shmeiwn pou anhkoun sto cluster

    public:
        Cluster(); //constructor
        Cluster(ClusterNode<T>* centr); //constructor
        ~Cluster(); //destructor

        Node<ClusterNode<T>*>* getPoints(); //epistrefei thn lista me ta shmeia
        int getNoPoints(); //epistrefei ton ari8mo twn shmeiwn pou anhkoun sto cluster
        ClusterNode<T>* getCentroid(); //epistrefei to kentroeides

        void setCentroid(ClusterNode<T>* centr); //ana8esh kentroeidous sto cluster

        void insertPoint(ClusterNode<T>* point); //eisagwgh shmeiou sto cluster 
        void deletePoint(Node<ClusterNode<T>*>* delPoint); //diagrafh tou node apo thn lista

        void emptyCluster(); //sunarthsh gia na adeiasei to cluster apo stoixeia
      
};



#endif
