#include "kMedoids.h"
#include <string>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include "dataTypes.h"
#include "distancesCalculations.h"
#include "generalFunctions.h"
#include "initialization.h"

using namespace std;

template <class T>
KMedoids<T>::KMedoids(List<T>* Points, int numberOfClusters, int noHashTables = 5, int noHashFunction = 4)
{
    noClusters = numberOfClusters;
    clusters = new Cluster<T>*[noClusters];
    for(int i =0; i < noClusters; i++)
    {
        clusters[i] = new Cluster<T>();
    }

    lshPoints = new List<ClusterNode<T>*>();
    for(Node<T>* i = Points->get_begin(); i != NULL; i = i->get_next()) //kanw ta simeia Points typou hamming, vector ktl se ClusterNode pou exei ola ta dedomena kai id pou xreiazomai
    {
        lshPoints->insertEnd(new ClusterNode<T>(i->get_data()));
    }

    distances = new Distance<ClusterNode<T>*>(lshPoints); //klash gia ton ypologismo twn apostasewn
    lsh = new LSH<ClusterNode<T>*>(lshPoints, noHashTables, noHashFunction, distances);
}

template <class T>
KMedoids<T>::~KMedoids()
{
    ClusterNode<T>* tmp;

    while((tmp = lshPoints->deleteFirstNode()) != NULL)
    {
        delete tmp;
    }
    delete lshPoints;

    for(int i =0; i< noClusters; i++)
    {
        delete clusters[i];
    }

    delete lsh;
    delete distances;
    delete[] clusters;
}

template <class T>
void KMedoids<T>::initializationPP() //initialization k-medoids++
{
    Initialization<T>* init = new Initialization<T>(this);
    init->initializationPP(lshPoints);
    delete init;
}

template <class T>
void KMedoids<T>::initializationConcetrate() //initialization concentrate
{
    Initialization<T>* init = new Initialization<T>(this);
    init->initializationConcetrate(lshPoints);
    delete init;
}

template <class T>
List<ClusterNode<T>*>* KMedoids<T>::getLSHPoints()
{
    return lshPoints;
}

template <class T>

Distance<ClusterNode<T>*>* KMedoids<T>::getDistance()
{
    return distances;
}

template <class T>
Cluster<T>** KMedoids<T>::getClusters()
{
    return clusters;
}

template <class T>
int KMedoids<T>::getNoClusters()
{
    return noClusters;
}

template <class T>
void KMedoids<T>::run(InitializationType initial, AssignmentType Assign, UpdateType update, int s = 2) //ektelesh twn sunduasmwn twn me8odwn
{
    if(update == Clarans) //sto clarans kalw thn dikia tou sinarthsh
    {
        cout<<"clarans"<<endl;
        clarans(initial, Assign, s);
        return;
    }

    Initialization<T>* init = new Initialization<T>(this);
    if(initial == InitializationPP)
    {
        cout<<"PLUS PLUS init"<<endl;
        init->initializationPP(lshPoints);
    }
    else //InitializationConcentrate
    {
        cout<<"Concetrate init"<<endl;
        init->initializationConcetrate(lshPoints);
    }
    delete init;

    if(Assign == PamAssign)
    {
        cout<<"Pam assign"<<endl;
        pamAssign();
    }
    else //LSHAssign
    {
        cout<<"LSH assign"<<endl;
        lshAssign();
    }

    if(update == Lloyds)
    {
        cout<<"Lloyds"<<endl;
        updateAllLloyds(Assign);
    }
}

template <class T>
void KMedoids<T>::clarans(InitializationType initial, AssignmentType Assign, int s=2)//clarans
{
    if(s<=0) return;

    Initialization<T>* init;
    ClusterNode<T>** bestCentroids = new ClusterNode<T>*[noClusters];
    double minObjective = std::numeric_limits<double>::max();
    double tmpObjective;

    for(int i =0; i < s; i++)
    {
        init = new Initialization<T>(this);

        if(initial == InitializationPP) //anathetw ta arxika cetroids
        {
            init->initializationPP(lshPoints);
        }
        else //InitializationConcentrate
        {
            init->initializationConcetrate(lshPoints);
        }
        delete init;

        if(Assign == PamAssign) //kanw assign ta shmeia
        {
            pamAssign();
        }
        else //LSHAssign
        {
            lshAssign();
        }

        updateClarans(max(0.12* noClusters * (lshPoints->getSize() - noClusters), 250.0), Assign);
        tmpObjective = calculateObjective();

        if(tmpObjective < minObjective)
        {
            minObjective = tmpObjective;
            for(int i = 0; i< noClusters; i++)
            {
                bestCentroids[i] = clusters[i]->getCentroid();
            }
        }
    }

    for(int i = 0; i< noClusters; i++) //vazw ta kalytera kentra
    {
        clusters[i]->setCentroid(bestCentroids[i]);
    }

    if(Assign == PamAssign) //ta kanw assign
    {
        pamAssign();
    }
    else //LSHAssign
    {
        lshAssign();
    }

    delete[] bestCentroids;
}

template <class T>
double KMedoids<T>::minDistance() //mikroterh apostash metaksh dyo centroids
{
    double minDist = std::numeric_limits<double>::max();
    double tmpDist;

    for(int i =0; i < noClusters-1; i++) //pairnw kathe kentro
    {
        for(int j =i+1; j < noClusters; j++)
        {
            tmpDist = distances->distance(clusters[i]->getCentroid(), clusters[j]->getCentroid()); //ypologizw thn metaksy tous apostash

            if(tmpDist < minDist)
            {
                minDist = tmpDist;
            }
        }

    }

    return minDist;
}

template <class T>
double KMedoids<T>::calculateObjective() //ypologizei thn obective timh
{
    double sum =0;

    for(int i =0; i < noClusters; i++)
    {
        for(Node<ClusterNode<T>*>* j = clusters[i]->getPoints(); j != NULL; j = j->get_next())
        {
            sum += j->get_data()->getDistance();
        }
    }

    return sum;
}

template <class T>
bool KMedoids<T>::isCentroid(ClusterNode<T>* point) //elenxei an to shmeio einai kentro
{
    for(int i=0; i < noClusters; i++)
    {
        if(clusters[i]->getCentroid() == point)
        {
            return true;
        }
    }
    return false;
}
/*
template <class T>
double KMedoids<T>::meanVector(int centroidPos, int dimension) //no point to use it in hamming
{
    double sum = 0;

    for(Node<ClusterNode<T>*>* n=clusters[centroidPos]->getPoints(); n!=NULL; n = n->get_next()) //gia ka8e shmeio pou anhkei sto cluster
    {
        sum += n->get_data()->get_coordinance(dimension);
    }

    return sum;
}

template <class T>
void KMedoids<T>::updateLloydsKMeans() //update a la lloyd
{
    int dimensions = clusters[0]->getCentroid()->getPoint()->get_dimensions();
	double meanVectorArray[dimensions];
	ClusterNode<T>* minCentroid = NULL;

	for(int i=0; i<noClusters; i++) //gia ka8e cluster
	{
        for(int j =0; j < dimensions; j++)
        {
            meanVectorArray[j] = meanVector(i, j);
        }

        //new Vector euclideanNode
    }
}
*/
template <class T>
void KMedoids<T>::updateLloyds() //update a la lloyd
{
	double sum=0;
	double centroidDistance;
	double min;
	ClusterNode<T>* minCentroid = NULL;

	for(int i=0; i<noClusters; i++) //gia ka8e cluster
	{
        centroidDistance = 0;
        min = std::numeric_limits<double>::max();

		for(Node<ClusterNode<T>*>* n=clusters[i]->getPoints(); n!=NULL; n = n->get_next()) //gia ka8e shmeio pou anhkei sto cluster
		{
            centroidDistance += distances->distance(clusters[i]->getCentroid(), n->get_data()); //h apostash tou shmeiou apo to kentro tou
            sum=0;
			for(Node<ClusterNode<T>*>* m=clusters[i]->getPoints(); m!=NULL; m = m->get_next())
			{
				sum += distances->distance(n->get_data(),m->get_data()); //apostash shmeiou apo alla shmeia tou kentrou
			}
			sum +=distances->distance(n->get_data(),clusters[i]->getCentroid()); //apostash shmeiou apo to kentro tou

			if(sum<min) //kratame to mikrotero
			{
				min=sum;
                minCentroid = n->get_data();
			}
		}

        if(centroidDistance < min) //mporei to kentro na einai hdh to kalytero medoid
        {
            minCentroid = clusters[i]->getCentroid();
        }

		clusters[i]->setCentroid(minCentroid); //kanoume to medoid kentro
	}
}

template <class T>
void KMedoids<T>::updateAllLloyds(AssignmentType Assign) //sunduasmos tou lloyd me pam assign h lsh assign
{
    ClusterNode<T>** bestCentroids = new ClusterNode<T>*[noClusters];
    double bestObjective;
    double tmpObjective;

    tmpObjective = calculateObjective();

    do
    {
        for(int i =0; i < noClusters; i++)
        {
            bestCentroids[i] = clusters[i]->getCentroid();
        }

        bestObjective = tmpObjective;
        updateLloyds();

        if(Assign == PamAssign)
        {
            pamAssign();
        }
        else //LSHAssign
        {
            lshAssign();
        }

    }while((tmpObjective = calculateObjective()) < bestObjective); //ama h objective timh ginei megalhterh stamata

    for(int i =0; i < noClusters; i++) //anathesh ton kalyterwn kentrwn
    {
        clusters[i]->setCentroid(bestCentroids[i]);
    }

    if(Assign == PamAssign)
    {
        pamAssign();
    }
    else //LSHAssign
    {
        lshAssign();
    }

    delete[] bestCentroids;
}

template <class T>
void KMedoids<T>::updateClarans(int Q, AssignmentType Assign) //sunduasmos clarans me pam h lsh
{
    Cluster<T>* randomCentroid = NULL;
    ClusterNode<T>* previousCentroid;
    ClusterNode<T>* randomPoint;

    ClusterNode<T>* bestCenter = NULL;
    Cluster<T>* bestCluster = NULL;

    double bestObjectValue = calculateObjective();
    double tmpObjectValue;

    for(int i =0; i < Q; i++)
    {
        randomCentroid = clusters[randomNumberInt(0, noClusters-1)]; //pairnw tyxaio kentro

        do
        {
            randomPoint = lshPoints->getPoint(randomNumberInt(0, lshPoints->getSize()-1)); //pairnw tyxaio simeio

        }while(isCentroid(randomPoint));  //dn prepei na einai kentro

        previousCentroid = randomCentroid->getCentroid(); //krataw to prohgoumeno kentro
        randomCentroid->setCentroid(randomPoint); //thetw to tyxaio shmeio kentro

        if(Assign == PamAssign) //kanw assign
        {
            pamAssign();
        }
        else //LSHAssign
        {
            lshAssign();
        }

        tmpObjectValue = calculateObjective();

        if(tmpObjectValue < bestObjectValue)
        {
            bestObjectValue = tmpObjectValue;
            bestCenter = randomPoint;
            bestCluster = randomCentroid;
        }

        randomCentroid->setCentroid(previousCentroid); //ksanavazw to proigoumeno kentro

    }

    if(bestCenter != NULL)//an allakse vazw to neo kentro
    {
        bestCluster->setCentroid(bestCenter);
    }

    if(Assign == PamAssign)
    {
        pamAssign();
    }
    else //LSHAssign
    {
        lshAssign();
    }

}

template <class T>
LshAssingNode<T>* elementExists(List<LshAssingNode<T>*>* tmpList, ClusterNode<T>* key) //vriskei an yparxei to key sthn lista apo LshAssignNodes xrhsimopoieitai apo to lshAssign
{
    for(Node<LshAssingNode<T>*>* i = tmpList->get_begin(); i!= NULL; i = i->get_next())
    {
        if(i->get_data()->getPoint() == key)
        {
            return i->get_data(); //epistrefw to stoixeio LshAssignNode
        }
    }

    return NULL;
}

template <class T>
bool elementExists(List<T>* tmpList, T key) //elenxei an yparxei hdh to stoixeio sthn lista
{
    for(Node<T>* i = tmpList->get_begin(); i!= NULL; i = i->get_next())
    {
        if(i->get_data() == key)
        {
            return true;
        }
    }

    return false;
}

template <class T>
void KMedoids<T>::pam(ClusterNode<T>** bestCentroids) //pam
{
    bool changed;

    double tmpObjectiveFunction;
    double bestObjectiveFunction;

    Initialization<T>* init = new Initialization<T>(this); //arxikopoihsh twn kentrwn
    init->initializationRandom(lshPoints);
    delete init;

    for(int i =0; i < noClusters; i++)
    {
        bestCentroids[i] = clusters[i]->getCentroid();
    }

    pamAssign();
    bestObjectiveFunction = calculateObjective();
    do
    {
        changed = false;
        for(int i =0; i < noClusters; i++)
        {
            clusters[i]->setCentroid(bestCentroids[i]);
        }

        for(int i =0; i < noClusters; i++) //dokimazw kathe syndiasmo
        {
            for(Node<ClusterNode<T>*>* j = lshPoints->get_begin(); j != NULL; j = j->get_next())
            {
                if(isCentroid(j->get_data())) continue; //ama einai kentro to agnow

                clusters[i]->setCentroid(j->get_data());
                pamAssign();

                tmpObjectiveFunction = calculateObjective();

                if(tmpObjectiveFunction < bestObjectiveFunction)
                {
                    bestObjectiveFunction = tmpObjectiveFunction;
                    bestCentroids[i] = j->get_data();
                    changed = true; //allakse to objective opote tha ksanaektelestei to loop
                }

                clusters[i]->setCentroid(bestCentroids[i]); //krataw to kalytero centroid gia to cluster i
            }
        }

    }while(changed == true); //ama dn vrethike oute ena kalytero synolo kentrwn stamata thn diadikasia


}

//einai o ypologismos tou Dj dn xrhsimopoieitai giati dn dinei swsta apotelesmata
/*
template <class T>
double KMedoids<T>::calculateDifference(int cluster, ClusterNode<T>* prevCenter)
{
    double tmpDist;
    double sumDist = 0;
    double minDist = std::numeric_limits<double>::max();

    for(int i = 0; i < noClusters; i++)
    {
        for(Node<ClusterNode<T>*>* j = clusters[i]->getPoints(); j != NULL; j = j->get_next())
        {
            tmpDist =  distances->distance(clusters[cluster]->getCentroid(), j->get_data()); //clusters[i]->getCentroid() einai to t

            if(i == cluster)
            {
                if(tmpDist <= j->get_data()->getSecDistance())
                {
                    sumDist += tmpDist;
                }
                else
                {
                    sumDist += j->get_data()->getSecDistance();
                }
            }
            else
            {
                if(tmpDist < j->get_data()->getDistance())
                {
                    sumDist += tmpDist;
                }
                else
                {
                    sumDist += j->get_data()->getDistance();
                }

            }
        }
    }

    for(int i=0; i < noClusters; i++)
    {
        tmpDist = distances->distance(prevCenter, clusters[i]->getCentroid());
        if(tmpDist < minDist)
        {
            minDist = tmpDist;
        }
    }
    sumDist += minDist;

    return sumDist;
}
*/

template <class T>
void KMedoids<T>::clara(int s = 5) //clara
{
    ClusterNode<T>** bestCentroids = new ClusterNode<T>*[noClusters];
    ClusterNode<T>** currentBestCentroids = new ClusterNode<T>*[noClusters];
    List<ClusterNode<T>*>* randomList;
    List<ClusterNode<T>*>* fullList;
    int randomSample = 40 + 2*noClusters;;

    ClusterNode<T>* tmpNode;

    double bestObjective = std::numeric_limits<double>::max();
    double currentObjective;

    fullList = lshPoints;
    for(int i =0; i <s; i++)
    {
        randomList = new List<ClusterNode<T>*>();

        for(int j =0; j < randomSample; j++) //dimiourgei tyxaio yposynolo ths listas
        {
            do
            {
               tmpNode = fullList->getPoint(randomNumberInt(0, fullList->getSize()-1));
            }while(elementExists(randomList, tmpNode)); //ama to exw parei hdh pairnw kainourgio shmeio

            randomList->insertEnd(tmpNode);
        }

        //lshPoints = randomList; //nea lista einai to yposynolo
        pam(currentBestCentroids); //klhsh pam

        for(int j =0; j < noClusters; j++) //vazoume ta kentra pou vrethikan
        {
            clusters[j]->setCentroid(currentBestCentroids[j]);
        }

        lshPoints = fullList; //pairnoume thn arxikh lista
        pamAssign();
        currentObjective = calculateObjective();

        if(currentObjective < bestObjective) //an edwse kalyterh objective krataw ta kentra
        {
            bestObjective = currentObjective;
            for(int j = 0; j < noClusters; j++)
            {
                bestCentroids[j] = currentBestCentroids[j];
            }
        }

        delete randomList;
    }

    for(int j =0; j < noClusters; j++) //vazoume ta kalytera kentra
    {
        clusters[j]->setCentroid(bestCentroids[j]);
    }

    pamAssign();

    delete[] bestCentroids;
    delete[] currentBestCentroids;

}


template <class T>
void KMedoids<T>::pamAssign() //pam assign
{
    NodeDistance<int> minCentroid;
	double dist;

	for(int i =0; i < noClusters; i++) //adeiazw ta clusters
    {
        clusters[i]->emptyCluster();
    }

	for(Node<ClusterNode<T>*>* i = lshPoints->get_begin(); i!= NULL; i = i->get_next())
	{
        minCentroid.dist = std::numeric_limits<double>::max();
        if(this->isCentroid(i->get_data())) continue;

		for(int j=0;j<noClusters;j++) //sunexeia mhdenismos mesa sthn for gia na mhn menoun kataloipa apo prohgoumenous upologismous
		{
            dist = distances->distance(i->get_data(),clusters[j]->getCentroid());
            if(dist < minCentroid.dist)
            {
                minCentroid.dist = dist;
                minCentroid.item = j;
            }
		}

        /*if(i->get_data() == clusters[minCentroid.item]->getCentroid())
        {
            continue;
        }*/
        clusters[minCentroid.item]->insertPoint(i->get_data());

		//i->get_data()->setSecCentroid(clusters[y2]->getCentroid());
		i->get_data()->setDistance(minCentroid.dist);
		//i->get_data()->setSecDistance(dist[1].value);
	}
}

template <class T>
void KMedoids<T>::lshAssign() //lsh assign
{
    List<LshAssingNode<T>*>* resultList = new List<LshAssingNode<T>*>(); //krataei ta apotelesmata twn anathesewn
    List<ClusterNode<T>*>* tmpList = new List<ClusterNode<T>*>(); //apotelesmata rangequery gia kathe centroid
    double radius;
    double tmpDist;
    LshAssingNode<T>* tmpAssignNode;
    ClusterNode<T>* tmpClusterNode;
    int counterAssigPoints, totalAssigned = 0;


    for(Node<ClusterNode<T>*>* i = lshPoints->get_begin(); i != NULL; i = i->get_next()) //thetw gia anazhthsh ola ta stoixeia
    {
        i->get_data()->setChecked(false);
    }

    for(int i =0; i < noClusters; i++) //afairw ta kentra apo thn lista gia psaksimo
    {
        clusters[i]->getCentroid()->setChecked(true); //thetw to flag true giati dn thelw na elenxw ta kentra

        clusters[i]->emptyCluster(); //adeiazw ta clusters
    }

    radius = minDistance(); //elaxisth apostash metaksh twn kentrwn

    do
    {
        counterAssigPoints = 0;

        while((tmpAssignNode = resultList->deleteFirstNode()) != NULL) //adeiasma listas apotelesmatwn
        {
            delete tmpAssignNode;
        }

        for(int i =0; i < noClusters; i++)
        {

            while((tmpClusterNode = tmpList->deleteFirstNode()) != NULL){} //adeiazw thn lista gia epistrofh tou radius query

            lsh->rangeSearch(clusters[i]->getCentroid(), tmpList, radius); //rangeQuery

            for(Node<ClusterNode<T>*>* j = tmpList->get_begin(); j!= NULL; j = j ->get_next())
            {
                tmpDist = distances->distance(j->get_data(), clusters[i]->getCentroid());
                tmpAssignNode = elementExists(resultList, j->get_data());

                if(tmpAssignNode != NULL) //yparxei hdh
                {
                    if(j->get_data()->getDistance() < tmpAssignNode->getDistance()) //an h apostash tou neou shmeiou einai mikroterh tote to ananewnw
                    {
                        counterAssigPoints++; //nea anathesh
                        tmpAssignNode->setCluster(clusters[i]); //allazw to kentro
                        tmpAssignNode->setDistance(tmpDist); //allazw thn apostash
                    }
                }
                else
                {
                    counterAssigPoints++; //nea anathesh
                    totalAssigned++;

                    //dhmiourgia neou node me ta stoixeia pou exoun anakalifthei
                    resultList->insertEnd(new LshAssingNode<T>(clusters[i], j->get_data(), tmpDist));
                }
            }
        }
        radius *=2;

        for(Node<LshAssingNode<T>*>* j = resultList->get_begin(); j != NULL; j = j->get_next()) //anathesh twn shmeiwn sta clusters
        {
            j->get_data()->getPoint()->setChecked(true); //shmeiwse oti exei hdh vrethei
            j->get_data()->getPoint()->setDistance(j->get_data()->getDistance()); //vazw thn apostash
            j->get_data()->getCluster()->insertPoint(j->get_data()->getPoint()); //topothethsh tou shmeio sto cluster
        }

    }while(counterAssigPoints != 0 && lshPoints->getSize()-noClusters > totalAssigned); //ama parw ola ta shmeia 'h dn mou epistefontai kainourgia tote stamata

    if(totalAssigned != lshPoints->getSize() - noClusters) //gia osa simeia dn exoun anatethei
    {
        for(Node<ClusterNode<T>*>* i = lshPoints->get_begin(); i != NULL; i = i->get_next())
        {
            if(i->get_data()->getChecked()== false) //pairnw osa simeia dn exoun anatethei
            {
                if(isCentroid(i->get_data())) continue; //dn theloume na anathesoume ta kentra

                Cluster<T>* centr;
                double minDistance = std::numeric_limits<double>::max();

                for(int j =0; j < noClusters; j++) //vriskoume to poio kontino kentro kai anathetoume to shmeio
                {
                    double tmpDist = distances->distance(i->get_data(), clusters[j]->getCentroid());
                    if(tmpDist < minDistance)
                    {
                        minDistance = tmpDist;
                        centr = clusters[j];
                    }
                }

                i->get_data()->setDistance(minDistance);
                centr->insertPoint(i->get_data());
            }
        }

    }

    //delete listes
    while((tmpAssignNode = resultList->deleteFirstNode()) != NULL)
    {
        delete tmpAssignNode;
    }

    delete tmpList;
    delete resultList;
}

template <class T>
double KMedoids<T>::calculateSilhouette(ClusterNode<T>* point, int cluster) //calculate silhouette for each point
{
    double a, b, s;
    double minDist = std::numeric_limits<double>::max();
    double tmpDist;
    int neighbor;
    int size=0;
    double dist = 0;

    if(clusters[cluster]->getNoPoints() == 0) //empty cluster has silhouette -1
    {
        return -1;
    }

    for(Node<ClusterNode<T>*>* m=clusters[cluster]->getPoints();m!=NULL; m = m->get_next())
    {
        dist +=distances->distance(point,m->get_data());
    }
    dist += distances->distance(point,clusters[cluster]->getCentroid());

    size=clusters[cluster]->getNoPoints();
    size++;
    a=dist/size;
    dist=0; //mporw na balw tous mhdenismous kai sto telos

    for(int j=0;j<noClusters;j++)
    {
        if(j == cluster) continue;

        tmpDist = distances->distance(point,clusters[j]->getCentroid()); //idia me to point->get_distance() ?
        if(tmpDist < minDist)
        {
            neighbor = j;
            minDist = tmpDist;
        }
    }

    for(Node<ClusterNode<T>*>* m=clusters[neighbor]->getPoints();m!=NULL;m = m->get_next())
    {
        dist += distances->distance(point,m->get_data());
    }
    dist += distances->distance(point,clusters[neighbor]->getCentroid());
    size=clusters[neighbor]->getNoPoints();
    b=dist/size;
    dist=0;

    if(a<b)
    {
        s=1-a/b;
    }
    else if(a==b)
    {
        s=0;
    }
    else if(a>b)
    {
        s=b/a-1;
    }

    return s;
}

template <class T>
void KMedoids<T>::sihouette(double* resultArray) //upologismos tou sunolikou silhouette
{
	int i;
	double sum;
	double sil;
	int cl; //cluster size of points.

	for(i=0;i<noClusters;i++)
	{
        sil=0;
		cl=0;

		for(Node<ClusterNode<T>*>* n=clusters[i]->getPoints(); n!=NULL;n = n->get_next())
		{
            sil += calculateSilhouette(n->get_data(), i);
		}
		sil += calculateSilhouette(clusters[i]->getCentroid(), i);

		cl=clusters[i]->getNoPoints() +1;
		resultArray[i]=sil/cl; //silhouette ana cluster
	}

    sum = 0;
	for(int i =0; i < noClusters; i++)
	{
        sum += resultArray[i];
	}

    resultArray[noClusters] = sum/noClusters;
}

template <class T> //topo8ethsh tou apotelesmatos sto arxeio e3odou
void KMedoids<T>::printClusters(string& outputFile, InitializationType init, AssignmentType Assign, UpdateType update, double time, bool complete = false, bool Clara = false)
{
    ofstream outFile;
    double* sil = new double[noClusters+1]; //sto teleutaio stoixeio tha grafei to synoliko silhouette

    outFile.open(outputFile.c_str(), ofstream::out | ofstream::app);

    if(Clara == true)
    {
         outFile << "Algorithm: CLARA"<<endl;
    }
    else
    {
        outFile << "Algorithm: " <<"I"<<init+1<<"A"<<Assign+1<<"U"<<update+1<<endl;
    }

    for(int i =0; i < noClusters; i++) //grafoume to centroid kai to megethos twn clusters
    {
        stringstream ss;
        ss << i+1;
        outFile << "CLUSTER-"<<ss.str()<<" {size: "<<clusters[i]->getNoPoints()<<", medoid: "<<clusters[i]->getCentroid()->getPoint()->get_string()<<"}"<<endl;
    }
    //grafoume to time
    outFile<<"clustering_time: "<<time<<endl;

    //grafoume to silhouete
    sihouette(sil);
    outFile << "Silhouette: [";
    for(int i =0; i < noClusters+1; i++)
    {
        outFile<<sil[i];
        if(i != noClusters)
        {
            outFile<<", ";
        }
    }
    outFile<<"]"<<endl;

    delete[] sil;
    outFile<<endl;

    if(!complete)
    {
        outFile.close();
        return;
    }

    //engrafh twn stoixeion twn clusters
    for(int i =0; i < noClusters; i++)
    {
        stringstream ss;
        ss << i+1;
        outFile << "CLUSTER-"<<ss.str()<<" {";
        for(Node<ClusterNode<T>*>* j = clusters[i]->getPoints(); j != NULL; j = j->get_next())
        {
            outFile<<j->get_data()->getPoint()->get_string();
            if(j->get_next() != NULL)
            {
                outFile<<", ";
            }
        }

        outFile<<"}"<<endl;
    }

    outFile<<endl;
    outFile.close();
}

template <class T>
void KMedoids<T>::neighbors(ClusterNode<T>* point, List<ClusterNode<T>*>* resultList)
{
    Cluster<T>* cluster = NULL;

    if(resultList == NULL || point == NULL) return;

    for(int i=0; i<noClusters; i++) //search the cluster which contains the point item
	{
        if(point->getPoint()->get_string() == clusters[i]->getCentroid()->getPoint()->get_string())  //if its a center
        {
            cluster = clusters[i];
        }
        for(Node<ClusterNode<T>*>* j=clusters[i]->getPoints(); j!=NULL; j = j->get_next()) //if it belongs to the center
		{
            if(point->getPoint()->get_string() == j->get_data()->getPoint()->get_string())
            {
                cluster = clusters[i];
            }
		}
    }

    if(cluster == NULL) return; //didnt found it return

    for(Node<ClusterNode<T>*>* j=cluster->getPoints(); j!=NULL; j = j->get_next()) //if it belongs to the center
    {
        if(point->getPoint()->get_string() == j->get_data()->getPoint()->get_string()) continue; //if it is the point continue to next

        resultList->insertEnd(j->get_data());
    }

    if(point->getPoint()->get_string() != cluster->getCentroid()->getPoint()->get_string())  //if its not a center add the center to the list
    {
        resultList->insertEnd(cluster->getCentroid());
    }

}

template <class T>
void BestNoClusters(List<T>* inputList, int& noClusters, int iterations, InitializationType initial, AssignmentType Assign, UpdateType update)
{
    double bestSilhouette = -2;//min value is -1
    KMedoids<T>* clusters;
    int randomNOCluster;
    double* silhouetteResult = new double[(int)sqrt(inputList->getSize())+3];

    for(int i = 0; i < iterations; i++)
    {
        //randomNOCluster = 10;
        randomNOCluster = randomNumberInt(2, sqrt(inputList->getSize()+2));
        cout<<"i  is "<<i<<endl;
        clusters = new KMedoids<T>(inputList, randomNOCluster, 5, 4);  //new KMedoids<T>(lshPoints, i, lsh, distanceMatrix);cout<<"E"<<endl;

        clusters->run(initial, Assign, update, 2);
        clusters->sihouette(silhouetteResult);

        if(silhouetteResult[randomNOCluster] > bestSilhouette)
        {
            bestSilhouette = silhouetteResult[randomNOCluster];
            noClusters = randomNOCluster;
        }

        delete clusters;
    }


    delete[] silhouetteResult;
}


template class KMedoids<Vector* >;
template class KMedoids<Hamming* >;
template class KMedoids<EuclideanNode* >;
template class KMedoids<MatrixPoint* >;

template void BestNoClusters(List<Hamming*>* inputList, int& noClusters, int iterations, InitializationType initial, AssignmentType Assign, UpdateType update);
template void BestNoClusters(List<EuclideanNode*>* inputList, int& noClusters, int iterations, InitializationType initial, AssignmentType Assign, UpdateType update);
template void BestNoClusters(List<Vector*>* inputList, int& noClusters, int iterations, InitializationType initial, AssignmentType Assign, UpdateType update);

