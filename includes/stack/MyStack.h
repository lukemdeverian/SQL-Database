#ifndef MYSTACK_H
#define MYSTACK_H
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"


template <class ITEM_TYPE>
class Stack
{
public:
    class Iterator{
    public:
        friend class Stack;                              //give access to list to access _ptr
        Iterator(){
            _ptr = nullptr;
        }                                     //default ctor
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;
        }                   //Point Iterator to where p is pointing to

        ITEM_TYPE& operator *(){
            return _ptr->_item;
        }                       //dereference operator
        ITEM_TYPE* operator ->(){ //addres of the item.
            return &(_ptr->_item);
        }                       //member access operator

        bool is_null(){
            return (_ptr == nullptr);
        }                                 //true if _ptr is NULL

        friend bool operator !=(const Iterator& left, const Iterator& right){
            if(left._ptr == right._ptr){
                return false;
            } return true;
        } //true if left != right

        friend bool operator ==(const Iterator& left, const Iterator& right){
            if(left._ptr == right._ptr){
                return true;
            } return false;
        } //true if left == right

        Iterator& operator++(){
            if(_ptr != nullptr){
                _ptr = _ptr->_next;
            }
            return *this;
        }                         //member operator: ++it; or ++it = new_value

        friend Iterator operator++(Iterator& it, int unused){
            Iterator hold;
            hold = it;
            if(it._ptr != nullptr){
                it._ptr = it._ptr->_next;
            }
            return hold;
        }         //friend operator: it++

    private:
        node<ITEM_TYPE>* _ptr;                          //pointer being encapsulated
    };      
    Stack(){
        _head_ptr = nullptr;
        _size = 0;
    }                                                     //CTOR
                                                                //BIG 3:
    ~Stack(){
        _clear_list(_head_ptr);
        _size = 0;
    }
    Stack(const Stack<ITEM_TYPE> &copyThis){
        _head_ptr = _copy_list<ITEM_TYPE>(copyThis._head_ptr);
        _size = copyThis._size;
    }
    Stack& operator =(const Stack& RHS){
        if(this == &RHS){
        return *this;
        }
        _clear_list(_head_ptr);
        _size = RHS._size;
        _head_ptr = _copy_list(RHS._head_ptr);

        return *this;
    }

    //int size();
    Iterator insert_head(ITEM_TYPE i){
        _size++;
        return _insert_head(_head_ptr, i);
        // return Iterator(_insert_head(_head_ptr, i));
        // Iterator return_me(_insert_head(_head_ptr, i));
        // return return_me;
    }                           //insert at the head of list
    void push(ITEM_TYPE i){
        _size++;
        _insert_head(_head_ptr, i);
        // return Iterator(_insert_head(_head_ptr, i));
        // Iterator return_me(_insert_head(_head_ptr, i));
        // return return_me;
    } 
    Iterator insert_after(ITEM_TYPE i, Iterator iMarker){
        _size++;
        return _insert_after(_head_ptr, iMarker._ptr, i);
        // Iterator return_me(_insert_after(_head_ptr, iMarker._ptr, i));
        // return return_me;
    }    //insert after marker
    Iterator insert_before(ITEM_TYPE i, Iterator iMarker){
        _size++;
        return _insert_before(_head_ptr, iMarker._ptr, i);
    }   //insert before marker
    Iterator insert_sorted(ITEM_TYPE i){
        _size++;
        return _insert_sorted(_head_ptr, i);
    }                         //insert in a sorted list

    ITEM_TYPE Delete(Stack<ITEM_TYPE>::Iterator iMarker){
        return _delete_node(_head_ptr, iMarker._ptr);
    }        //delete node pointed to by marker
    void Print() const{
        _print_list(_head_ptr);
    }
    Iterator search(const ITEM_TYPE &key){
        return _search_list(_head_ptr, key);
    }                      //return Iterator to node [key]
    Iterator prev(Iterator iMarker){
        return _previous_node(_head_ptr, iMarker._ptr);
    }                            //previous node to marker

    ITEM_TYPE& operator[](int index){
        return _at(_head_ptr, index);
    }                           //return item at position index
    Iterator begin() const{
        Iterator beginning(_head_ptr);
        return beginning;
    }                                     //Iterator to the head node
    Iterator end() const{
        Iterator ending(nullptr);
        return ending;
    }                                       //Iterator to NULL
    Iterator last_node() const{
        return _last_node(_head_ptr);
    }                                  //Iterator to the last node

    bool empty()const {return _head_ptr == NULL;}
    template <class U>                                          //Note the template arg U
    friend ostream& operator <<(ostream& outs, const Stack<U>& l);
    int size()const { return _size; }

    ITEM_TYPE top(){
        if(_head_ptr == nullptr){
            return ITEM_TYPE();
        }
        return _head_ptr->_item;
    }

    ITEM_TYPE pop(){
        if(_head_ptr == nullptr){
            return ITEM_TYPE();
        }
        
        ITEM_TYPE item = _head_ptr->_item;

        _delete_head(_head_ptr);
        _size--;
        return item;
    }



private:
    node<ITEM_TYPE>* _head_ptr;
    int _size;
};

template <class U>                                          //Note the template arg U
ostream& operator <<(ostream& outs, const Stack<U>& l){
    return _print_list(l._head_ptr);
}

#endif