#ifndef Included_KMEDOIDS_H
#define Included_KMEDOIDS_H

#include "centroid.h"
#include "List.h"
#include "lsh.h"

enum InitializationType //orismos duo pi8anwn timwn gia thn initialization (xrhsimeuei sto na ginoun oi aparaithtoi sunduasmoi gia thn askhsh)
{
  InitializationPP,
  InitializationConcentrate
};

enum AssignmentType //orismos duo pi8anwn timwn gia thn Assignment
{
  PamAssign,
  LSHAssign
};

enum UpdateType //orismos duo pi8anwn timwn gia thn Update
{
  Lloyds,
  Clarans
};

template <class T>
class LshAssingNode //orismos class LshAssingNode eidikh gia th xrhsh ths me8odou LSH
{
    private:
        ClusterNode<T>* point;
        Cluster<T>* cluster;
        double distance;
    public:
        LshAssingNode(Cluster<T>* Cluster, ClusterNode<T>* Point, double dist)
        {
            cluster = Cluster;
            point = Point;
            distance = dist;

        }

        ClusterNode<T>* getPoint()
        {
            return point;
        }

        Cluster<T>* getCluster()
        {
            return cluster;
        }
        double getDistance()
        {
            return distance;
        }

        void setCluster(Cluster<T>* Cluster)
        {
            cluster = Cluster;
        }

        void setDistance(double dist)
        {
            distance = dist;
        }
};

template <class T>
class KMedoids //class KMedoids gia thn sustadopoihsh twn dedomenwn
{
    private:
        Cluster<T>** clusters; //pinakas apo clusters
        int noClusters; //ari8mos twn clusters
        List<ClusterNode<T>*>* lshPoints; //shmeia twn clusters
        LSH<ClusterNode<T>*>* lsh; //lsh class
        Distance<ClusterNode<T>*>* distances; //pinakas apostasewn

        double minDistance(); //elaxisth apostash dyo centroid

    public:
        KMedoids(List<T>* Points, int numberOfClusters, int noHashTables, int noHashFunction); //constructor
        ~KMedoids(); //destructor

        Cluster<T>** getClusters(); //epistrofh deikth sthn arxh tou pinaka twn clusters
        int getNoClusters(); //epistrofh tou ari8mou twn clusters

        void initializationPP(); //arxikopoihsh k-medoids++
        void initializationConcetrate(); //arxikopoihsh concentrate

        Distance<ClusterNode<T>*>* getDistance(); //epistrofh tou class distance gia ypologismo apostasewn dyo shmeiwn
        List<ClusterNode<T>*>* getLSHPoints(); //epistrofh ths listas twn shmeiwn pou dwthikan ws eisodo
        void run(InitializationType initial, AssignmentType Assign, UpdateType update, int s); //ektelesh twn me8odwn
        void clarans(InitializationType initial, AssignmentType Assign, int s); // ektelesh clarans me tous pi8anous sunduasmous initialization kai assignmet

        //assign
        void pamAssign();//me8odos pam
        void lshAssign(); //me8odos lsh

        void printClusters(std::string& outputFile, InitializationType init, AssignmentType Assign, UpdateType update, double time, bool complete, bool Clara); //ektupwsh twn clusters sto output file

        double calculateObjective(); //upologismos objective function
        double calculateDifference(int cluster, ClusterNode<T>* prevCenter); //upologismos diaforas, gia Dj
        bool isCentroid(ClusterNode<T>* point); //boolean elegxos an to shmeio einai kentroeides


        void updateClarans(int Q, AssignmentType Assign); //me8odos update clarans
        void updateLloyds(); //me8odos update lloyd's
        void updateAllLloyds(AssignmentType Assign); //sunduasmos me tis pi8anes me8odous assign

        void sihouette(double* resultArray); //silhouette
        double calculateSilhouette(ClusterNode<T>* point, int cluster); //upologismos tou silhouette gia to ekastote shmeio
        void pam(ClusterNode<T>** bestCentroids); // pam

        void clara(int s); //me8odos clara

        void neighbors(ClusterNode<T>* point, List<ClusterNode<T>*>* resultList);
};

template <class T>
void BestNoClusters(List<T>* inputList, int& noClusters, int iterations, InitializationType initial, AssignmentType Assign, UpdateType update);

#endif
