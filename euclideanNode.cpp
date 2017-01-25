#include "euclideanNode.h"

//kwdikas twn synarthsewn pou anhkoun sthn class EuclideanNode 

EuclideanNode::EuclideanNode(Vector* Data)
{
    data = Data;
}

EuclideanNode::EuclideanNode(Vector* Data, int id)
{
    data = Data;
    ID = id;
}

EuclideanNode::~EuclideanNode()
{
}

Vector* EuclideanNode::get_vector()
{
    return data;
}

int EuclideanNode::get_ID()
{
    return ID;
}

std::string EuclideanNode::get_string()
{
    return data->get_string();
}

void EuclideanNode::set_ID(int id)
{
    ID = id;
}
