#ifndef Included_ClusterNode_H
#define Included_ClusterNode_H

template<class T>
class ClusterNode
{
    private:
        int id; //id xrhsimopoieitai ston upologismo apostashs
        T point; //shmeio
        ClusterNode<T>* secondCentroid; //deutero kontinotero centroid
        double distance; //apostash apo to centroid sto cluster pou anhkei
		double secDistance; //distance apo to deutero centroid

		bool checked; //xrhsimopoieitai sto lsh assign

    public:
        ClusterNode(T data); //constructor
        ClusterNode(T data, double dist); //constructor
        ~ClusterNode(); //destructor

        int getID(); //epistrofh tou id
        T getPoint(); //epistrofh tou shmeiou
        double getDistance(); //epistrofh apostashs apo kentroeides
        ClusterNode<T>* getSecCentroid(); //epistrofh deuterou kentroeidous
		double getSecDistance(); //epistrofh apostashs apo deutero kentroeides
		bool getChecked(); //epistrefei thn timh tou checked

        void setID(int ID); //orismos timhs id
        void setDistance(double dist);  //orismos apostashs apo kentroeides
        void setSecCentroid(ClusterNode<T>* centroid2);
        void setSecDistance(double secDist); //orismos apostashs apo deutero kentroeides
   		void setChecked(bool check); //8etei timh sto checked, true an exei elegx8ei to stoixeio

};

#endif
