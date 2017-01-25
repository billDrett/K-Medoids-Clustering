#include <iostream>
#include <stdlib.h>
#include "List.h"
#include "dataTypes.h"
#include "Node.h"
#include "centroid.h"
#include "lsh.h"
#include "clusterNode.h"
#include "euclideanNode.h"
#include "kMedoids.cpp"
#include "readFile.h"
#include "generalFunctions.h"

using namespace std;

template <class T>
void callClusterFunctions(KMedoids<T>* kmedoid, string& outputFile, bool complete, int s);

int main(int argc, char *argv[])
{
    ifstream inFile;
    ifstream configFile;
    ofstream outFile;
    string inputFile="", configurationFile="", outputFile = "";
    bool complete = false;
    string type;
    int noClusters;
    int L = 5, K = 4;
    int claransSample = -1; //Q, plithos stoixeiwn pou tha dokimasei to clarans
    int claransIterations = 2; //s, plithos epanalipsewn
    Hamming* tmpHamming;
    Vector* tmpVector;
    MatrixPoint* tmpMatrix;
    List<Vector*>* myList1;
    List<Hamming*>* myList2;
    List<MatrixPoint*>* myList3;

    initialize(); //arxikopoiei thn srand

    if (argc < 7 || argc > 8)
    {
        cout<<"Please give all attributes –d <input file> –c <configuration file> -o <output file> -complete"<<endl;
        exit(1);
    }

    for(int i=1; i < argc; i++)
    {
        if(strcmp(argv[i], "-complete") == 0)
        {
            complete = true;
        }

        if(i+1 != argc) // an dn einai to teleutaio
        {
            if(strcmp(argv[i], "-d") == 0)
            {
                inputFile = argv[i+1];
            }
            else if(strcmp(argv[i], "-c") == 0)
            {
                configurationFile = argv[i+1];
            }
            else if(strcmp(argv[i], "-o") == 0)
            {
                outputFile = argv[i+1];
            }
        }
    }
    if(inputFile == ""|| configurationFile==""||outFile=="") //kapoio argument dn dwthhke swsta
    {
        perror("Arguments dont match");
        exit(2);
    }

    inFile.open(inputFile.c_str());
    configFile.open(configurationFile.c_str());
    outFile.open(outputFile.c_str());

    if(inFile.is_open() == false || configFile.is_open() == false ||outFile.is_open()==false) //elenxos an ontws ta arxeia yparxoun
    {
        perror("Cant open files");
        exit(3);
    }
    outFile.close();

    myList1 = new List<Vector*>();
    myList2 = new List<Hamming*>();
    myList3 = new List<MatrixPoint*>();

    //diavasma tou input file
    if(readFile(inFile, type, myList1, myList2, myList3))
    {
        perror("Error data of input file");
        exit(4);
    }

    inFile.close();



    //diavasma tou config file
    if(readFileConfig(configFile, noClusters, K, L, claransSample, claransIterations))
    {
        perror("Error data of config file");
        exit(4);
    }

    if(claransSample == -1) //an dn diavastike to claransSample pairnei default timh
    {
        int listSize = max(myList1->getSize(), myList2->getSize());
        listSize = max(listSize, myList3->getSize());
        claransSample = max(0.12* noClusters * (listSize - noClusters), 250.0);
    }

    configFile.close();

    if(type =="hamming") //ta dedomena einai hamming
    {
        cout<<"hamming"<<endl;
        KMedoids<Hamming*>* kmedoids = new KMedoids<Hamming*>(myList2, noClusters, L, K);

        callClusterFunctions(kmedoids, outputFile, complete, claransIterations);

        delete kmedoids;
    }
    else if(type == "cosine") //ta dedomena einai cosine
    {
        cout<<"cosine"<<endl;
        KMedoids<Vector*>* kmedoids = new KMedoids<Vector*>(myList1, noClusters, L, K);

        callClusterFunctions(kmedoids, outputFile, complete, claransIterations);

        delete kmedoids;
    }
    else if(type == "euclidean") //ta dedomena einai euclidean
    {
        cout<<"euclidean"<<endl;
        EuclideanNode* tmpEuclid;

        List<EuclideanNode*>* euclideanList = new List<EuclideanNode*>(); //ftiaxnw thn eukleidio node
        for(Node<Vector*>*i=myList1->get_begin(); i!=NULL; i =i->get_next())
        {
            euclideanList->insertEnd(new EuclideanNode(i->get_data()));
        }

        KMedoids<EuclideanNode*>* kmedoids = new KMedoids<EuclideanNode*>(euclideanList, noClusters, L, K);

        callClusterFunctions(kmedoids, outputFile, complete, claransIterations);

        while((tmpEuclid = euclideanList->deleteFirstNode()) != NULL) //diagrafw ta euclideanNode
        {
            delete tmpEuclid;
        }

        delete euclideanList;
        delete kmedoids;
    }
    else if(type == "matrix")
    {
        cout<<"matrix"<<endl;
        KMedoids<MatrixPoint*>* kmedoids = new KMedoids<MatrixPoint*>(myList3, noClusters, L, K);

        callClusterFunctions(kmedoids, outputFile, complete, claransIterations);

        delete kmedoids;
    }

    //diagrafw tis listes me ta stoixeia
    while((tmpHamming = myList2->deleteFirstNode()) != NULL)
    {
        delete tmpHamming;
    }

    while((tmpVector = myList1->deleteFirstNode()) != NULL)
    {
        delete tmpVector;
    }

    while((tmpMatrix = myList3->deleteFirstNode()) != NULL)
    {
        delete tmpMatrix;
    }

    delete myList1;
    delete myList2;
    delete myList3;
}

template <class T>
void callClusterFunctions(KMedoids<T>* kmedoid, string& outputFile, bool complete, int s) //klhsh olwn twn syndiasmwn
{
    double time;
    clock_t begin, end;

    begin = clock();
    kmedoid->run(InitializationPP, PamAssign, Lloyds);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationPP, PamAssign, Lloyds, time, complete); //grapsimo sto arxeio


    begin = clock();
    kmedoid->run(InitializationPP, PamAssign, Clarans, s);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationPP, PamAssign, Clarans, time, complete);

    begin = clock();
    kmedoid->run(InitializationPP, LSHAssign, Lloyds);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationPP, LSHAssign, Lloyds, time, complete);

    begin = clock();
    kmedoid->run(InitializationPP, LSHAssign, Clarans, s);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationPP, LSHAssign, Clarans, time, complete);

    begin = clock();
    kmedoid->run(InitializationConcentrate, PamAssign, Lloyds);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationConcentrate, PamAssign, Lloyds, time, complete);

    begin = clock();
    kmedoid->run(InitializationConcentrate, PamAssign, Clarans, s);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationConcentrate, PamAssign, Clarans, time, complete);

    begin = clock();
    kmedoid->run(InitializationConcentrate, LSHAssign, Lloyds);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationConcentrate, LSHAssign, Lloyds, time, complete);

    begin = clock();
    kmedoid->run(InitializationConcentrate, LSHAssign, Clarans, s);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationConcentrate, LSHAssign, Clarans, time, complete);

    begin = clock();
    kmedoid->clara();
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    kmedoid->printClusters(outputFile, InitializationPP, PamAssign, Clarans, time, complete, true); //den paizoun rolo ta arguments gia InitializationPP, PamAssign, Clarans afou valame true to teleutaio bool
}

