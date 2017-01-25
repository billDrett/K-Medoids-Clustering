#ifndef Included_INITIALIZATION_H
#define Included_INITIALIZATION_H

#include "kMedoids.h"
#include "centroid.h"
#include "clusterNode.h"
#include "List.h"

template <class T>
class Initialization //class gia thn arxikopoihsh twn clusters, periexei ths 2 me8odous arxikopoihshs (k-medoids++ kai concentrate) kai mia sunarthsh pou xrhsimopoieitai sto clara
{
    private:
        KMedoids<T>* kMedoids;

        int binarySearch(double* distances,  double searchPoint, int start, int end); //sunarthsh gia to k-medoids++
    public:
        Initialization(KMedoids<T>* kMedoid); //constructor
        ~Initialization(); //destructor

        void initializationPP(List<ClusterNode<T>*>* points); //arxikopoihsh me k-medoids++
        void initializationConcetrate(List<ClusterNode<T>*>* points); //arxikopoihsh me concentrate
        void initializationRandom(List<ClusterNode<T>*>* points); //sunarthsh gia th me8odo clara
};

#endif
