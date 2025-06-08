#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H
#include <iostream>
#include <iomanip>
#include "../node/node.h"
#include <assert.h>
using namespace std;

    //Linked List General Functions:

    template <typename ITEM_TYPE>
    void _delete_head(node<ITEM_TYPE> *&head);

    template <typename ITEM_TYPE>
    ostream& _print_list(node<ITEM_TYPE>* head, ostream& outs = cout);

    //recursive fun! :)
    // template <typename ITEM_TYPE>
    // void _print_list_backwards(node<ITEM_TYPE> *head);

    //return ptr to key or NULL
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _search_list(node<ITEM_TYPE>* head, ITEM_TYPE key);


    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE> *&head, ITEM_TYPE insert_this);

    //insert after ptr
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head, node<ITEM_TYPE> *after_this, ITEM_TYPE insert_this);

    //insert before ptr
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* before_this, ITEM_TYPE insert_this);

    //ptr to previous node
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prev_to_this);

    //delete, return item
    template <typename ITEM_TYPE>
    ITEM_TYPE _delete_node(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* delete_this);

    //duplicate the list...
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _copy_list(node<ITEM_TYPE>* head);

    //duplicate list and return the last node of the copy
    template <typename ITEM_TYPE>
    node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE>* &dest, node<ITEM_TYPE> *src);


    //delete all the nodes
    template <typename ITEM_TYPE>
    void _clear_list(node<ITEM_TYPE>*& head);

    //_item at this position
    template <typename ITEM_TYPE>
    ITEM_TYPE& _at(node<ITEM_TYPE>* head, int pos);

    // template <typename ITEM_TYPE>
    // node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head, ITEM_TYPE i);

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head);

    //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    //      Sorted List Routines. order: 0: ascending, order: other: descending
    //                              Assume a Sorted List
    //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    template <typename ITEM_TYPE>  //insert
    node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);

    //insert or add if a dup
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);
    //node after which this item goes order: 0 ascending
    /*template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _where_this_goes(node<ITEM_TYPE>* head, ITEM_TYPE item, bool ascending=true);*/
    //Last Node in the list
    //template <typename ITEM_TYPE>
    //node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head);            //never use this function.


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ====================================== DEFINITIONS ===========================================
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template<typename ITEM_TYPE>
    ostream& _print_list(node<ITEM_TYPE>* head, ostream& outs){
        node<ITEM_TYPE>* walker = head;  //creates a walker at the head of the nodes
        while(walker != nullptr){
            cout << *walker;          //goes through until the walker is nullptr, prints out the dereferenced pointer
            walker = walker->_next;  //increments the walker
        }
        cout << "END.";   //demonstrates that the end has been reached

        return outs;
    }
    
    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _search_list(node<ITEM_TYPE>* head, ITEM_TYPE key){
        node<ITEM_TYPE>* walker = head;  //creates a walker at the head

        while(walker != nullptr && walker->_item != key){  //will continue to increment until either nullptr has been reached or the key has been found
            walker = walker->_next;   //increments the walker
        }

        return walker; //returns the walker
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE> *&head, ITEM_TYPE insert_this){
        // 1. create a new crate
        node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this, head);   //this function was given to us from fun with linked lists
        // 2. point him to where headptr is pointing to.
        // temp->_next = head;
        // 3. point headptr to this new guy
        head = temp;
        return head;
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prev_to_this){

        if(head == nullptr || prev_to_this == nullptr){  //if the list is empty, or the parameter node is nullptr, return nullptr
            return nullptr;
        }
        if(head == prev_to_this){  //if the parameter node is the head, return nullptr since there is no previous node
            return nullptr;
        }
        node<ITEM_TYPE>* walker = head;  //creates a walker at the head
        while(walker != nullptr && walker->_next != prev_to_this){  //continues to increment until the next is nullptr or the next is the parameter node
            walker = walker->_next;  //increment the walker
        }
        return walker;  //return the walker
    }

    template <typename ITEM_TYPE>
    ITEM_TYPE _delete_node(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* delete_this){
        ITEM_TYPE return_me = delete_this->_item;  //returns the element of the deleted node
        if(head == nullptr || delete_this == nullptr){
            return ITEM_TYPE();  //if the list is empty, return a blank node
        }
        if(head == delete_this){ //if the head is the deleted node, call the distinctive delete head function and return the element
            _delete_head(head);
            return return_me;
        }
        node<ITEM_TYPE>* prev = _previous_node(head, delete_this);
        if(prev && prev->_next == delete_this){
            prev->_next = delete_this->_next;
            delete delete_this;
        }
        // prev->_next = delete_this->_next;  //sync up the previous node with the deleted node's next index
        // delete delete_this; //delete the deleted node
        return return_me;  //return its element
    }

    template <typename ITEM_TYPE>
    void _delete_head(node<ITEM_TYPE> *&head){
        if(head == nullptr){  //return if the list is empty
            return;
        }
        node<ITEM_TYPE>* deletedHead = head;  //deletedhead will be the head
        //cout << "\ndeleted head: " << *deletedHead << "\n\n";
        head = deletedHead->_next;  //change head to the next node
        delete deletedHead; //delete the old head
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head, node<ITEM_TYPE> *after_this, ITEM_TYPE insert_this){
        if(head == nullptr){
            return _insert_head<ITEM_TYPE>(head, insert_this);  //if the list in empty, just call insert head
        }

        node<ITEM_TYPE>* insertedNode = new node<ITEM_TYPE>(insert_this);  //create the new node
        insertedNode->_next = after_this->_next;  //sync the two nodes with the new node
        after_this->_next = insertedNode;

        return head;  //return head
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* before_this, ITEM_TYPE insert_this){

        if(before_this == nullptr){  //if before_this is an invalid index, return nullptr
            return nullptr;
        }

        if(head == nullptr || head == before_this){  //if the list is empty or before_this is the head, call insert_head
            return _insert_head(head, insert_this);
        }

        /*if(head == before_this){
            return _insert_head(head, insert_this);
        }*/

        node<ITEM_TYPE>* previous_Node = _previous_node<ITEM_TYPE>(head, before_this); //get the previous node

        // if(previous_Node == nullptr){
        //     return nullptr;
        // }

        return _insert_after<ITEM_TYPE>(head, previous_Node, insert_this);  //call insert_after but with the previous node
    }

    template <typename ITEM_TYPE>
    void _clear_list(node<ITEM_TYPE>*& head){
        while(head != nullptr){
            _delete_head(head);  //will continue to iterate until head is nullptr
        }
        head = nullptr;  //im pretty sure this is redundant
    }

    template <typename ITEM_TYPE>
    ITEM_TYPE& _at(node<ITEM_TYPE>* head, int pos){
        node<ITEM_TYPE>* walker = head;  //create a walker at the head
        int i = 0;  //tracks for int pos
        while(walker != nullptr && i != pos){  //will continue to reiterate until either walker is nullptr or i = pos
            walker = walker->_next;
            i++;   //increment walker and i
        }
        assert(i == pos && "Index is out of bounds!\n");
        return walker->_item;  //return the item
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _copy_list(node<ITEM_TYPE>* head){
        if(head == nullptr){
            return nullptr;   //if the list is empty, return nullptr
        }
        node<ITEM_TYPE>* copiedHead = nullptr;  //create a new head will nullptr
        _insert_head<ITEM_TYPE>(copiedHead, head->_item);  //copy the first head node, we can do this since we know that the list is not empty
        node<ITEM_TYPE>* walker = head->_next;
        node<ITEM_TYPE>* copiedWalker = copiedHead;  //create walkers for both the copied node and the original
        while(walker != nullptr){
            _insert_after<ITEM_TYPE>(copiedHead, copiedWalker, walker->_item); //will continue to insert after until walker is nullptr
            
            copiedWalker = copiedWalker->_next;
            walker = walker->_next;   //increment walkers
        }
        return copiedHead;  //return the newly copied head node
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head, ITEM_TYPE i, bool ascending){
        if(ascending){
            if(head == nullptr ){
                return _insert_head(head, i);
            }
            if(i < head->_item){
                return _insert_head(head, i);
            }
            
            node<ITEM_TYPE>* walker = head;
            while(walker->_next != nullptr && walker->_next->_item < i){
                walker = walker->_next;
            }
            
            return _insert_after(head, walker, i);
        }
        
        if(head == nullptr){
            return _insert_head(head, i);
        }
        if(i > head->_item){
            return _insert_head(head, i);
        }
        node<ITEM_TYPE>* walker = head;
        while(walker->_next != nullptr && walker->_next->_item > i){
            walker = walker->_next;
        }
        return _insert_after(head, walker, i);
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE>* &dest, node<ITEM_TYPE> *src){
        dest = _copy_list(src);  //same but dest will equal the previous function
        return dest;
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head){
        node<ITEM_TYPE>* walker = head;
        while(walker->_next != nullptr){
            walker = walker->_next;
        } return walker;
    }

    template <typename ITEM_TYPE>
    node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending){
        node<ITEM_TYPE>* walker = head;
        while(walker != nullptr && walker->_item != item){
            walker = walker->_next;
        }
        assert(walker != nullptr && "insert_sorted_and_add, walker is nullptr\n");
        walker->_item += item;

        return head;
    }

#endif