#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){

        }

        T operator *(){
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used){
            // if(it){
            //     key_ptr++;
            //     if(key_ptr >= it->data_count){
            //         key_ptr = 0;
            //         it = it->next;
            //     }
            // } return *this;
            int before = key_ptr;
            BPlusTree<T>* beforeIt = it;
            if(key_ptr < it->data_count-1){
                key_ptr++;
            } else{
                it = it->next;
                key_ptr = 0;
            } return Iterator(beforeIt, before);
        }
        Iterator operator++(){
            if(key_ptr < it->data_count -1 ){
                key_ptr++;
            } else{
                it = it->next;
                key_ptr = 0;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr ){
                return true;
            } return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs == rhs);
        }
        void print_Iterator(){
            if(it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false){
        child_count = 0;
        data_count = 0;
        next = nullptr;
        for(int i = 0; i < MAXIMUM + 2; i++){
            subset[i] = nullptr;
        }
    }
    BPlusTree(T *a, int size, bool dups = false): data_count(), child_count(0), dups_ok(dups){
        for(int i = 0; i < MAXIMUM + 2; i++){
            subset[i] = nullptr;
        }
        T* walker = a;
        for(int i = 0; i < size; i++){
            insert(*walker);
        }
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){
        data_count = 0;
        child_count = 0;
        copy_tree(other);
        dups_ok = other.dups_ok;
        // clear_tree(); //////////////dont think issue but could be
        // copy_array(data, other.data, data_count, other.data_count);

        // child_count = other.child_count;

        // for(int i = 0; i < child_count; i++){
        //     subset[i] = nullptr;
        // }

        // for(int i = 0; i < child_count; i++){
        //     subset[i] = new BPlusTree<T>(*other.subset[i]);
        // }

    }
    ~BPlusTree(){
        //cout << "THIS SHOULD BE PRINTING THIS BTREE SHOULD DIE!!!!";
        clear_tree();
        child_count = 0;
        data_count = 0;
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        if(this != &RHS){
            clear_tree();
            copy_tree(RHS);
            dups_ok = RHS.dups_ok;

            // copy_array(data, RHS.data, data_count, RHS.data_count);
            // child_count = RHS.child_count;
            // for(int i = 0; i < child_count; i++){
            //     subset[i] = new BPlusTree<T>(*RHS.subset[i]);
            // }
        }
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other){
        if(this == &other){
            return;
        }

        clear_tree();

        BPlusTree<T>* last = new BPlusTree<T>;
        copy_tree(other, last);
        last->next = nullptr;

        // copy_array(data, other.data, data_count, other.data_count);

        // child_count = other.child_count;
        // for(int i = 0; i < child_count; i++){
        //     subset[i] = new BPlusTree<T>();
        //     subset[i]->copy_tree(*other.subset[i]);
        // }
    }  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
        if(other.is_leaf()){
            copy_array(data, other.data, data_count, other.data_count);
            last_node->next = this;
            last_node = this;
        } else{
            copy_array(data, other.data, data_count, other.data_count);
            child_count = other.child_count;
            for(int i = 0; i < child_count; i++){
                subset[i] = new BPlusTree<T>();
                subset[i]->copy_tree(*(other.subset[i]), last_node);
            }
        }
    }

    void insert(const T& entry){
        loose_insert(entry);
        // cout << *this << endl;
        if(data_count > MAXIMUM){
            // cout << "hello";
            BPlusTree *growth_pointer = new BPlusTree<T>();
            copy_array(growth_pointer->data, data, growth_pointer->data_count, data_count);
            copy_array(growth_pointer->subset, subset, growth_pointer->child_count, child_count);
            data_count = 0;
            for(int i = 0; i < child_count; i++){
                subset[i] = nullptr;
            }
            child_count = 1;
            subset[0] = growth_pointer;
            fix_excess(0);
            // cout << "\ndata_count = " << data_count << endl;
        }
    }   //insert entry into the tree
    void remove(const T& entry){
        clear_tree();
    }   //remove entry from the tree
    void clear_tree(){
        for(int i = 0; i < child_count; i++){
            if(!is_leaf()){
                subset[i]->clear_tree();
            } 
            delete subset[i];
            subset[i] = nullptr;
        }

        next = nullptr;
        child_count = 0;
        data_count = 0;
    }             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const{
        for(int i = 0; i < data_count; i++){
            if(data[i] == entry){
                return true;
            }
        }
        if(is_leaf()){
            return false;
        }
        return subset[first_ge(data, data_count, entry)]->contains(entry);
    } //true if entry can be found
    T& get(const T& entry){
        //cout << "BPLUSTREE FROM GET(): \n\n" << *this << endl;
        if(!contains(entry)){
            insert(entry);
        } return get_existing(entry);
        
        //return *find_ptr(entry);
    }              //return a reference to entry
    const T& get(const T& entry)const{
        if(!contains(entry)){
            insert(entry);
        } return get_existing(entry);
        
        //return *find_ptr(entry);
    }   //return a reference to entry

    T& get_existing(const T& entry){
        int i = first_ge(data, data_count, entry);
        if(i < data_count && data[i] == entry){
            if(is_leaf()){
                return data[i];
            } return subset[i+1]->get_existing(entry);
        }
        if(!is_leaf()){
            return subset[i]->get_existing(entry);
        }
        assert(i < data_count && data[i] == entry && "you should not get here");
        cout << "hello";
        return data[i];
        //return *find_ptr(entry);
    }     //return a reference to entry
    Iterator find(const T& key){
        if(data[first_ge(data, data_count, key)] == key && !is_leaf()){
            return subset[first_ge(data, data_count, key) + 1]->find(key);
        }
        if(!is_leaf()){
            return subset[first_ge(data, data_count, key)]->find(key);
        }
        if(data[first_ge(data,data_count,key)] != key){
            return end();
        }
        return Iterator(this, first_ge(data, data_count, key));
    }         //return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key){
        int i = first_ge(data, data_count, key);

        if(!is_leaf()){
            return subset[i]->lower_bound(key);
        }

        if(i < data_count){
            return Iterator(this, i);
        } else{
            if(next != nullptr){
                return Iterator(next, 0);
            } else{
                return end();
            }
        }

    }  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key){
        int i = first_ge(data, data_count, key);

        if(!is_leaf()){
            return subset[i]->upper_bound(key);
        }

        while(i < data_count && data[i] <= key){
            i++;
        }

        if(i < data_count){
            return Iterator(this, i);
        } else{
            if(next != nullptr){
                return Iterator(next, 0);
            } else{
                return end();
            }
        }
    }  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const{
        int s = data_count;
        if(!is_leaf()){
            for(int i = 0; i < child_count; i++){
                s += subset[i]->size();
            }
        } return s;
    }                    //count the number of elements

    bool empty() const{
        if(data_count == 0 && child_count == 0){
            return true;
        }
        return false;
    }                  //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const{
        if(is_leaf())
        {
            for(int i = data_count - 1; i >= 0; i--)
            {
                outs << string(level, '\t') << data[i] << endl;
            }
        }
        else{
            subset[data_count]->print_tree(level + 1, outs);
            for(int i = data_count - 1; i >= 0; i--){
                outs << string(level, '\t') << data[i] << endl;
                subset[i]->print_tree(level + 1, outs);
            }
        }
    }
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        //cout << "size: " << print_me.size();
        return outs;
    }

    bool is_valid(){return true;}
    string in_order(){
        string result = "";

        if(is_leaf()){
            for(int i = 0; i < data_count; i++){
                result += to_string(data[i]) + "|";
            }
        }
        else{
            for(int i = 0; i < data_count; i++){
                result += subset[i]->in_order();
                result += to_string(data[i]) + "|";
            }
            result += subset[data_count]->in_order();
        }

        return result;
    }

    string pre_order(){
        //return "in work";
        string rm = "";
            for(int i = 0; i < data_count; i++){
                rm += to_string(data[i]) + "|";
                if(!is_leaf()){
                    rm += subset[i]->pre_order();
                }
            }
        if(!is_leaf()){
            rm += subset[data_count]->pre_order();
        }
        return rm;
    }
    string post_order(){
        //return "in work";
        string rm = "";
        if(is_leaf()){
            for(int i = 0; i < data_count; i++){
                rm += to_string(data[i]) + "|";
            }
            return rm;
        }
        rm += subset[0]->post_order();
        for(int i = 0; i < data_count; i++){
            rm += subset[i+1]->post_order();
            rm += to_string(data[i]) + "|";
        }
        return rm;
    }

    Iterator begin(){
        if(!is_leaf()){
            return subset[0]->begin();
        }
        return Iterator(this, 0);
    }
    Iterator end(){
        return Iterator(nullptr, 0);
    }
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry){
        if(data[first_ge(data, data_count, entry)] == entry && !is_leaf()){
            return subset[first_ge(data, data_count, entry) + 1]->find_ptr(entry);
        }
        if(!is_leaf()){
            return subset[first_ge(data, data_count, entry)]->find_ptr(entry);
        }
        return this;
    }         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry){
        int i = first_ge(data, data_count, entry);
        bool foundElement = i < data_count && data[i] == entry;
        if(foundElement){
            if(!is_leaf()){
                subset[i+1]->loose_insert(entry);
                fix_excess(i+1);
            } else{
                if(dups_ok){
                    data[i] =  data[i] + entry;
                } else{
                    data[i] =  data[i] + entry;
                }
            }
        } else{
            if(!is_leaf()){
                subset[i]->loose_insert(entry);
                fix_excess(i);
            } else{
                if(i < data_count){
                    insert_item(data, i, data_count, entry);
                } else{
                    attach_item(data, data_count, entry);
                }
            }
        }
    }   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i){

        if(subset[i]->data_count > MAXIMUM){
            BPlusTree* insertMe = new BPlusTree<T>();
            if(i+1 == child_count){
                attach_item(subset, child_count, insertMe);
            } else{
                insert_item(subset, i+1, child_count, insertMe);
            }
            split(subset[i]->data, subset[i]->data_count, insertMe->data, insertMe->data_count);
            split(subset[i]->subset, subset[i]->child_count, insertMe->subset, insertMe->child_count);
            T bringMeUp;
            detach_item(subset[i]->data, subset[i]->data_count, bringMeUp);
            if(first_ge(data, data_count, bringMeUp) < data_count){
                insert_item(data, first_ge(data, data_count, bringMeUp), data_count, bringMeUp);
            } else{
                attach_item(data, data_count, bringMeUp);
            }
            if(subset[i]->is_leaf()){
                insert_item(insertMe->data, 0, insertMe->data_count, bringMeUp);
                insertMe->next = subset[i]->next;
                subset[i]->next = insertMe;
            }
        }
    }              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};

#endif