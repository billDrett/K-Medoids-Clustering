#ifndef Included_Node_H
#define Included_Node_H

template <class T>
class Node
{
    private:
        Node<T>* next;
        Node<T>* prev;
        T data;
    public:
        Node(T Data);
        ~Node();

        Node<T>* get_next();
        Node<T>* get_prev();
        T get_data();

        void set_next(Node<T>* next1);
        void set_prev(Node<T>* prev1);
        void set_data(T Data);
};
#endif
