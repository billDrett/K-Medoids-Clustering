#include "distancesCalculations.h"
#include "dataTypes.h"
#include "euclideanNode.h"
#include "clusterNode.h"
#include <bitset>
#include <cmath>
#include <limits>

#include <eigen-eigen-f562a193118d/Eigen/Dense>
#include <eigen-eigen-f562a193118d/Eigen/SVD>
#include <eigen-eigen-f562a193118d/Eigen/LU>

using namespace std;
using namespace Eigen;
/*template<class T>
Distance<T>::Distance(List<T>* inputList)
{
    Node<T>* iNode;
    Node<T>* jNode;

    noItems = inputList->getSize();
    distances = new double*[noItems];


    iNode = inputList->get_begin();
    for(int i =0; i < noItems; i++)
    {
        distances[i] = new double[noItems];

        iNode->get_data()->setID(i);

        jNode = inputList->get_begin();
        for(int j =0; j < noItems; j++)
        {
            if(j < i)
            {
                distances[i][j] = distances[j][i];
            }
            else if(j== i)
            {
                distances[i][j] = 0;
            }
            else
            {
                distances[i][j] = this->distance(iNode->get_data()->getPoint(), jNode->get_data()->getPoint());
            }

            jNode = jNode->get_next();
        }

        iNode = iNode->get_next();
    }
}
*/

template<class T>
Distance<T>::Distance(List<T>* inputList)
{
    int count = 0;
    noItems = inputList->getSize();
    distances = new double*[noItems];

    for(int i =0; i < noItems; i++)
    {
        distances[i] = new double[noItems];
        for(int j =0; j < noItems; j++)
        {
            if(i ==j)
            {
                distances[i][j] = 0;
            }
            else
            {
                distances[i][j] = -1;
            }
        }
    }

    for(Node<T>* i = inputList->get_begin(); i != NULL; i = i->get_next())
    {
        i->get_data()->setID(count);
        count++;
    }
}

template<class T>
Distance<T>::~Distance()
{
    for(int i =0; i < noItems; i++)
    {
        delete[] distances[i];
    }

    delete[] distances;
}

template <class T>
double** Distance<T>::getArray()
{
    return distances;
}

template<class T>
int Distance<T>::distance(Hamming* point1, Hamming* point2) //ypologismos apostashs hamming
{
   	int distance=0;
   	bitset<64> str1;
   	bitset<64> str2;

   	point1->get_bitString(str1);
   	point2->get_bitString(str2);

	for(int i=0;i<point1->get_noBits();i++) //veltiwsh mexri point1 nobits
	{
		if(str1[i]!=str2[i]) //an einai diaforetika +1
		{
			distance++;
		}
	}

	return distance;
}

template<class T>
double Distance<T>::distance(Vector* point1, Vector* point2) //ypologismos apostashs cosine
{
    double innerProduct = 0, lengthPoint1 = 0, lenghtPoint2 =0;

    for(int i =0; i < point1->get_dimensions(); i++)
    {
        innerProduct += point1->get_coordinance(i)*point2->get_coordinance(i); //vriskw eswteriko ginomeno

        lengthPoint1 += pow(point1->get_coordinance(i), 2); //metro tou point1
        lenghtPoint2 += pow(point2->get_coordinance(i), 2); //metro tou point2
    }

    if(lengthPoint1 == 0|| lenghtPoint2 == 0)
    {
        cout<<"error sto cosine"<<endl;
    }

    return 1- innerProduct/(sqrt(lengthPoint1) * sqrt(lenghtPoint2));//return 1- innerProduct/(sqrt(lengthPoint1) * sqrt(lenghtPoint2));
}

template<class T>
double Distance<T>::distance(EuclideanNode* point1, EuclideanNode* point2) //ypologismos apostashs gia eukleidia
{
	double sum=0.0;

	for(int i=0;i<point1->get_vector()->get_dimensions();i++)
	{
		sum=sum + pow((point1->get_vector()->get_coordinance(i)-point2->get_vector()->get_coordinance(i)),2.0);
	}

	return sqrt(sum);
}

template<class T>
double Distance<T>::distance(MatrixPoint* point1, MatrixPoint* point2)
{//ypologizw thn apostash apo to querykey sto point1 kai oxi to anapodo giati to point1 dn exei dedomena gia to querykey

    if(point2->get_distance(point1->get_pos())  == 0) return std::numeric_limits<double>::max();

    return point2->get_distance(point1->get_pos());
}

template <class T>
double Distance<T>::distance(T point1, T point2)
{
    int i = point1->getID();
    int j = point2->getID();

    if(i < 0 || i >= noItems) return distance(point1->getPoint(), point2->getPoint()); //ektos oriwn tou pinaka
    if(j < 0 || j >= noItems) return distance(point1->getPoint(), point2->getPoint());

    if(distances[i][j] == -1)
    {
        distances[i][j] = distance(point1->getPoint(), point2->getPoint());
        distances[j][i] = distances[i][j];
        //cout<<i<<" "<<j<<" "<< distances[i][j]<<"dsdsd"<<endl;
    }
    else
    {
        //cout<<"OP"<<endl;
    }

    return distances[i][j];
}

template class Distance<ClusterNode<Vector*>*>;
template class Distance<ClusterNode<Hamming*>*>;
template class Distance<ClusterNode<EuclideanNode*>*>;
template class Distance<ClusterNode<MatrixPoint*>*>;

