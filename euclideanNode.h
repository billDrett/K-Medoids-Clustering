#ifndef Included_euclideanNode_H
#define Included_euclideanNode_H

#include "dataTypes.h"
class EuclideanNode //dhmiourgia class EuclideanNode giati prepei na apo8hkeuetai ena parapanw stoixeia apo auta pou einai sthn class vector gia to lsh
{
    private:
        Vector* data; //dedomena
        int ID; //apo8hkeush tou ID giati xreiazetai sto Lsh

    public:
        EuclideanNode(Vector* Data);
        EuclideanNode(Vector* Data, int id);
        ~EuclideanNode();

        Vector* get_vector();
        int get_ID();
        std::string get_string();

        void set_ID(int id);
};
#endif
