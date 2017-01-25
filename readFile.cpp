#include "readFile.h"
#include "Node.h"
#include <sstream>
#include <bitset>
#include "lsh.h"

using namespace std;

int readFileConfig(ifstream& inFile, int& noClusters, int& noHashFunctions, int& noHashTables, int& claransSample, int& claransIterations)
{
    string line;
    string tag;
    int value;
    bool error = true; //prepei na diavastei oposdipote to number_of_clusters:

    if(inFile.is_open() && (inFile)) //eisodos apo arxeio efoson yparxei
    {
        while(getline(inFile, line))
        {
            istringstream iss(line);

            iss>> tag >> value;
            if(!iss) break; //telos tou read

            if(tag == "number_of_clusters:")
            {
                noClusters = value;
                error = false;
            }
            else if(tag == "number_of_hash_functions:")
            {
                noHashFunctions = value;
            }
            else if(tag == "number_of_hash_tables:")
            {
                noHashTables = value;
            }
            else if(tag == "clarans_set_fraction:")
            {
                claransSample = value;
            }
            else if(tag == "clarans_iterations:")
            {
                claransIterations = value;
            }
        }

        if(!error) return 0; //an pirame to plithos ton clusters tote egine swsta to read

    }

    return 1;
}

int readFile(ifstream& inFile, string& type, List<Vector*>* myList1, List<Hamming*>* myList2, List<MatrixPoint*>* myList3)
{
    string line;
    string name, tag;
    double value;

    if(inFile.is_open() && (inFile)) //eisodos apo arxeio efoson yparxei
    {
        if(!getline(inFile, line)) return 1;// error sto read
        istringstream iss(line);

        iss>> tag >> type;
        if(!iss) return 1;
        if(tag != "@metric_space") return 1; //perimenoume auto to tag

        if(type == "vector")
        {
            if(!getline(inFile, line)) return 1;// error sto read

            istringstream iss(line);
            iss>> tag;
            if(!iss) return 1;
            if(tag=="@metric")
            {
                iss>> type;
                if(!iss) return 1;
                if(type !="euclidean" && type !="cosine") return 1; //lathos dedomena
                line = ""; //dn pira seira me dedomena
            }
            else //dn eixe grammi gia typo ara me default pairnw euclidean
            {
                type ="euclidean";
            }

            if(!readVectorFile(inFile, line, myList1)) //an den diavastikan swsta epistrefw sfalma
            {
                return 2;
            }
         }
        else if(type == "hamming")
        {
            if(!readHammingFile(inFile, myList2)) //an dn diavastei swsta epistrefw sfalma
            {
                return 3;
            }

        }
        else if(type == "matrix")
        {
            if(!readMatrixFile(inFile, myList3)) //an dn diavastei swsta epistrefw sfalma
            {
                return 4;
            }

        }
        else //error
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

bool readVectorFile(ifstream& inFile, string& line, List<Vector*>* myList)
{
    string name;
    double value;
    double* myArray;
    int dimensions;
    Vector* data;

    if(line == "") //den pira seira dedomenwn prin
    {
        if(!getline(inFile, line)) return false; //diavazw mia grammh
    }

    istringstream iss(line); //metrame tis diastaseis tou pinaka
    iss>>name;
    dimensions = 0;
    while(true)
    {
        iss>> value;
        if(!iss) break;
        dimensions++;
    }

    myArray = new double[dimensions];
    do
    {
        istringstream iss(line);

        iss>> name;
        for(int i =0; i <dimensions; i++)
        {
            iss>>value;
            if(!iss) return false;

            myArray[i] = value;
        }

        data = new Vector(name, dimensions, myArray);
        myList->insertEnd(data);

    }while(getline(inFile, line));

    delete[] myArray;

    return true;
}

bool readHammingFile(ifstream& inFile, List<Hamming*>* myList)
{
    string line, name, bitsString;
    Hamming* data;
    bitset<64> value;
    bool firstTime = true;
    int countBits;

    while(getline(inFile, line))
    {
        if(firstTime) //metrame to plithos twn bits
        {
            istringstream iss(line);

            iss>> name >> bitsString; //to value tha einai 64 bit
            if(!iss) return false;
            countBits = bitsString.length(); //plithos bits
            firstTime = false;

        }
        istringstream iss(line);

        iss>> name >> value; //to value tha einai 64 bit
        if(!iss) return false;

        data= new Hamming(name, value, countBits);
        myList->insertEnd(data);
    }

    return true;
}


bool readMatrixFile(ifstream& inFile, List<MatrixPoint*>* myList)
{
    string line, name, commaLine;
    int value, i, pos, items;
    int* tempArray;
    List<string> itemIds;
    MatrixPoint* point;

    if(!getline(inFile, line)) return false;
    istringstream iss(line);
    iss >> name;
    if(!iss) return false;

    if(name != "@items") return false;

    while(getline(iss, name, ',')) //ta ids xwrizontai me komma ara ta pairnw etsi
    {
        itemIds.insertEnd(name); //vazw ta Ids sthn lista
    }

    items = itemIds.getSize(); //ypologizw to synolo twn ids
    tempArray = new int[items];

    pos=0; //tha krataw kai pio stoixeio einai gia na mporw na vrw thn apostash kapoiou shmeiou apo auto
    while(getline(inFile, line)) //diavazw twra tis apostaseis
    {
        istringstream iss(line);
        for(int i = 0; i < items; i++)
        {
            iss>>value;
            if(!iss) return false; //diavastikan ligotera exoume kapoio error

            tempArray[i] = value;
        }
        point = new MatrixPoint(itemIds.get_begin()->get_data(), tempArray, items, pos);
        itemIds.deleteFirstNode(); //afairoume to prwto Id string apo thn lista
        myList->insertEnd(point);
        pos++;
    }

    delete[] tempArray;

    return true;
}

bool readMatrixQuery(std::ifstream& inFile, List<MatrixPoint*>* queryList)
{
    string name, line;
    MatrixPoint* data;
    int value, pos = 0, counter = 0;
    int* tmpArray;
    //bitset<64> value;

    if(!getline(inFile, line)) return false;
    istringstream iss(line);
    iss>>name;
    while(true) //metrame to plithos twn apostasewn
    {
        iss>> value;
        if(!iss) break;
        counter++;
    }
    tmpArray = new int[counter];
    pos = 0;
    do
    {
        istringstream iss(line);
        iss>>name;
        for(int i= 0; i <counter; i++)
        {
            iss>>value;
            if(!iss) return false;
            tmpArray[i] = value;
        }

        data = new MatrixPoint(name, tmpArray, counter, pos);
        queryList->insertEnd(data);
        pos++;
    }while(getline(inFile, line));

    delete[] tmpArray;

    return true;
}

bool readRadius(std::ifstream& inFile, double& radius)
{
    string line, name;
    if(inFile.is_open() && (inFile)) //eisodos apo arxeio efoson yparxei
    {
        if(!getline(inFile, line)) return false;// error sto read
        istringstream iss(line);

        iss>> name >> radius;
        if(!iss) return false;
        if(name != "Radius:") return false; //perimenoume auto to tag
    }

    return true;
}
