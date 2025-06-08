#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <iomanip>
using namespace std;

template <typename ITEM_TYPE>
struct node
{
public:
    node(const ITEM_TYPE& item = ITEM_TYPE(), node* next = nullptr):_item(item), _next(next){

    }

    friend std::ostream& operator <<(std::ostream& outs, const node<ITEM_TYPE> &printMe){
        outs << "[" << printMe._item << "] -> ";
        return outs;
    }
    ITEM_TYPE _item;
    node* _next;
};

#endif
