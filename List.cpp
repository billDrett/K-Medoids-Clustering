#include <iostream>
#include <cstdlib>
#include "List.h"
#include "Node.h"

//dn xreiazontai an kanw include to .cpp file
#include "dataTypes.h"
#include "euclideanNode.h"
#include "clusterNode.h"
#include "kMedoids.h"

using namespace std;

template <class T>
List<T>::List()
{
    listSize = 0;
    start = NULL;
    last = NULL;
}

template <class T>
List<T>::~List()
{
    Node<T>* tmp = start;
    Node<T>* del;

    while(tmp != NULL)//diagrafoume enan enan tous komvous
    {
        del = tmp;
        tmp = tmp->get_next();
        listSize--;
        delete del;
    }
}

template <class T>
bool List<T>::checkEmpty()
{
	return(listSize == 0);
}

template <class T>
int List<T>::getSize()
{
    return listSize;
}

template <class T>
Node<T>* List<T>::get_begin()
{
    return start;
}

template <class T>
Node<T>* List<T>::get_last()
{
    return last;
}

template <class T>
Node<T>* List<T>::insertBeginning(T item) //epistrefei to node pou dhmiourghthike
{
	Node<T>* node = new Node<T>(item);

	if(checkEmpty()) //an einai to monadiko stoixeio prepei na allaksw ton deikti start kai last
	{
	    start = node;
	    last = node;
	    listSize++;
		return node;
	}
	//alliws eisodos stin arxi
    node->set_next(start);
    start->set_prev(node);

    start = node;
    listSize++;

    return node;
}

template <class T>
Node<T>* List<T>::insertEnd(T item)
{
	Node<T>* node = new Node<T>(item);

	if(checkEmpty()) //an einai to monadiko stoixeio prepei na allaksw ton deikti start kai last
	{
	    start = node;
	    last = node;
	    listSize++;
		return node;
	}
	//alliws eisodos sto telos ths listas
    last->set_next(node);
    node->set_next(NULL);
    node->set_prev(last);

    last = node;
    listSize++;
    return node;
}

template <class T>
T List<T>::deleteFirstNode()
{
    Node<T>* tmp;
    T tmpAccount;
    if(checkEmpty())
    {
        return 0;
    }

    tmp = start;
    start = start->get_next();

    if(listSize == 1) last = NULL; //arxi kai telos deixnoun sto idio, epomenos i lista exei mono ena stoixeio
    else start->set_prev(NULL);

    tmpAccount = tmp->get_data();
    delete tmp;
    listSize--;
    return tmpAccount;
}

template <class T>
T List<T>::deleteLastNode()
{
    Node<T>* tmp;
    T tmpAccount;
    if(checkEmpty())
    {
        return 0;
    }

    if(listSize == 1) //an exei mono ena stoixeio tote prepei na allaksei kai o deiktis start
    {
        tmpAccount = start->get_data();
        delete start;
        start = NULL;
        last = NULL;
        listSize--;
        return tmpAccount;
    }

    tmpAccount = last->get_data();
    tmp = last;
    last = last->get_prev();
    last->set_next(NULL);

    delete tmp;
    listSize--;
    return tmpAccount;
}

template <class T>
T List<T>::deleteNode(Node<T>* node) //diagrafei tou sigekrimenou node
{
    Node<T>* tmp;
    T data;

    if(checkEmpty() == true)
    {
        return 0;
    }

    if(start == node) // an einai o prwtos 'h o monadikos komvos
    {
        return this->deleteFirstNode();
    }

    if(last == node) //an o teleutaios
    {
        return this->deleteLastNode();
    }

    //elenxos gia sfalma
    if(node->get_next() == NULL || node->get_prev()== NULL) return 0; //afou dn einai to prwto 'h teleutaio stoixeio dn prepei na einai null oi deiktes

    tmp = node->get_prev();
    tmp->set_next(node->get_next());
    node->get_next()->set_prev(tmp);

    data = node->get_data();
    listSize--;
    delete node;
    return data;
}

template <class T>
T List<T>::getPoint(int pos) //epistrefei to tyxaio simeio
{
    int count = 0;

    for(Node<T>* i = start; i != NULL; i = i->get_next())
    {
        if(count == pos)
        {
            return i->get_data();
        }

        count++;
    }
  }

template class List<int>;
template class List<double>;
template class List<string>;

template class List<Vector* >;
template class List<Hamming* >;
template class List<EuclideanNode* >;
template class List<MatrixPoint* >;

template class List<ClusterNode<Vector*>*>;
template class List<ClusterNode<Hamming*>*>;
template class List<ClusterNode<EuclideanNode*>*>;
template class List<ClusterNode<MatrixPoint*>*>;

template class List<LshAssingNode<Vector*>*>;
template class List<LshAssingNode<Hamming*>*>;
template class List<LshAssingNode<EuclideanNode*>*>;
template class List<LshAssingNode<MatrixPoint*>*>;
