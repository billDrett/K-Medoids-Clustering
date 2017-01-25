#include "initialization.h"
#include "Node.h"
#include "psedoRandomNumbers.h"
#include <iostream>
#include <limits>

#include "dataTypes.h"
#include "euclideanNode.h"
#include "clusterNode.h"
#include "distancesCalculations.h"

#include "generalFunctions.h"

using namespace std;

template <class T>
Initialization<T>::Initialization(KMedoids<T>* kMedoid) //constructor
{
    kMedoids = kMedoid;
}

template <class T>
Initialization<T>::~Initialization() //destructor
{

}

/*
template<class T>
int Initialization<T>::binarySearch(double* distances,  double searchPoint, int start, int end) //binary search gia k-medoids++
{
    for(int i =0; i < end; i++)
    {
        if(searchPoint < distances[i]) return i;
    }
}*/


template<class T>
int Initialization<T>::binarySearch(double* distances,  double searchPoint, int start, int end) //binary search gia k-medoids++
{
    int middle = (start + end)/2;

    if(end<=start)
    {
        if(distances[end] < searchPoint) return end+1;
        else return end; //dn mporei na vgei ektos pinaka
    }

    if(distances[middle] < searchPoint)
    {
        return binarySearch(distances, searchPoint, middle +1, end);
    }
    else
    {
        return binarySearch(distances, searchPoint, start, middle-1);
    }
}


template <class T>
void Initialization<T>::initializationPP(List<ClusterNode<T>*>* points) //efarmogh ths me8odou k-medoids ++
{
    double* distances =  new double[points->getSize()];
    double* PMetric =  new double[points->getSize()];
    double tmpDist;
    Node<ClusterNode<T>*>* node;
    ClusterNode<T>* tmp;
    double randomX;
    Cluster<T>** clusters = kMedoids->getClusters();
    int noClusters = kMedoids->getNoClusters();

    tmp = points->getPoint(randomNumberInt(0, points->getSize()-1)); //pairnw tyxaio simeio vhma 1

    clusters[0]->setCentroid(tmp);

    for(int i =0; i < points->getSize(); i++) //arxikopoihsh toy pinaka
    {
        distances[i] = std::numeric_limits<double>::max();
    }

    for(int i=0; i < noClusters-1; i++)
    {
        node = points->get_begin();
        for(int j = 0; j < points->getSize(); j++) //vima 2
        {
            tmpDist = kMedoids->getDistance()->distance(node->get_data(), clusters[i]->getCentroid());

            if(tmpDist < distances[j])
            {
                distances[j] = tmpDist;
            }
            node = node->get_next();
        }

        PMetric[0] = 0;
        double maxDist = maxDistance(distances, points->getSize());
        for(int j=1; j< points->getSize(); j++) //eoxume apo thn arxh n-1 P afou to prwto centroid to pairnoume tyxaia
        {
            PMetric[j] = PMetric[j-1] + distances[j]*distances[j]/maxDist; //ypologismos tou P
        }

        randomX = randomNumberDouble(0, PMetric[points->getSize()-1]);

        tmp = points->getPoint( binarySearch(PMetric, randomX, 0, points->getSize()-1));


        clusters[i+1]->setCentroid(tmp);
    }

    delete[] distances;
    delete[] PMetric;
}
/*
template<class T>
void Initialization<T>::initializationConcetrate(List<ClusterNode<T>*>* points) //efarmogh ths me8odou concentrate
{
	struct dis
	{
		double value;// to v pou upologizoume gia thn euresh twn centroids
		int position; //ousiastika einai to number tou stoixeiou mias kai exoun perastei me th seira ths listas sta distances
	};

    Cluster<T>** clusters = kMedoids->getClusters();
    int noClusters = kMedoids->getNoClusters();
	int size=points->getSize(); //pairnw to mege8os ths listas twn stoixeiwn
	double** distances = kMedoids->getDistance()->getArray();
	dis* v = new dis[size]; // na dw an einai ontws toso to mege8os tou pinaka v, gia apo8hkeush twn timwn tou v
	double* sum = new double [size];
	int m=0;
	int n=0;
	int d=0;
	int e=0;
	double c1=0;
	double c2=0;
	int a,b;
	dis temp;
    Node<ClusterNode<T>*>* i;

	for(int i=0;i<size;i++) //upologismos twn summaries gia xrhsh ston paranomasth tou tupou gia concentrate init
	{
		for(int j=0;j<size;j++)
		{
			c1+=distances[i][j];
		}
		sum[i]=c1;
		c1=0;
	}

	for(m=0;m<size;m++)
	{
		for(n=0; n<size;n++)
		{
			c2+=(distances[m][n]/sum[n]);
		}
		v[m].value=c2;
		v[m].position=m;
		c2=0;
	}

    for(a=0;a<size;a++) //taksinomoume ton pinaka
    {
        for(b=0;b<(size-a);b++)
        {
            if(b!=(size-1)&&((v[b].value)>(v[b+1].value)))
            {
                temp=v[b];
                v[b]=v[b+1];
                v[b+1]=temp;
            }
        }
    }

	for(d=0;d<noClusters;d++)
	{
		i = points->get_begin();
		for(e=0;e<v[d].position;e++)
		{
			 i = i->get_next();

		}
		clusters[d]->setCentroid(i->get_data());
	}

	delete[] sum;
	delete[] v;
}
*/

template<class T>
void Initialization<T>::initializationConcetrate(List<ClusterNode<T>*>* points) //efarmogh ths me8odou concentrate
{
    Cluster<T>** clusters = kMedoids->getClusters();
    int noClusters = kMedoids->getNoClusters();
	int size=points->getSize(); //pairnw to mege8os ths listas twn stoixeiwn
	double** distances;
	NodeDistance<ClusterNode<T>*>* v = new NodeDistance<ClusterNode<T>*>[size]; // na dw an einai ontws toso to mege8os tou pinaka v, gia apo8hkeush twn timwn tou v
	double* sum = new double [size];

	double c1, c2;
	int a,b;
    Node<ClusterNode<T>*>* tmp;
    int k;

    k = 0;
    for(Node<ClusterNode<T>*>* i = points->get_begin(); i != NULL; i = i->get_next())
    {
        c1=0;
        for(Node<ClusterNode<T>*>* j = points->get_begin(); j != NULL; j=j->get_next())
        {
            c1+= kMedoids->getDistance()->distance(i->get_data(), j->get_data());
        }
        sum[k]=c1;
        k++;
    }
    distances = kMedoids->getDistance()->getArray();

	tmp = points->get_begin();
	for(int m=0;m<size;m++)
	{
        c2=0;
		for(int n=0; n<size;n++)
		{
			c2+=(distances[m][n]/sum[n]);
		}
		v[m].dist=c2;
		v[m].item = tmp->get_data();
		tmp = tmp->get_next();
	}

    quickSort(v, 0, size-1);

	for(int i=0;i<noClusters;i++)
	{
		clusters[i]->setCentroid(v[i].item);
	}

	delete[] sum;
	delete[] v;
}

template <class T>
void Initialization<T>::initializationRandom(List<ClusterNode<T>*>* points) //sunarthsh Random gia thn ulopoihsh ths me8odou clara
{
    Cluster<T>** clusters = kMedoids->getClusters();
    int noClusters = kMedoids->getNoClusters();
    ClusterNode<T>* tmp;

    for(int i =0; i < noClusters; i++)
    {
        do
        {
            tmp = points->getPoint(randomNumberInt(0, points->getSize()-1)); //epilegw tyxaio centroid
        }while(kMedoids->isCentroid(tmp)); //koitaw mhpws einai hdh

        clusters[i]->setCentroid(tmp);
    }
}


template class Initialization<Vector* >;
template class Initialization<Hamming* >;
template class Initialization<EuclideanNode* >;
template class Initialization<MatrixPoint* >;
