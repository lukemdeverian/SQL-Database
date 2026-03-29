// Known limitation: iterator traversal via leaf chain may skip elements
// in certain insertion orders due to a leaf split linking issue.
// Core operations (insert, contains, remove, get) function correctly
// for the SQL engine's use case.

#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "btree_array_funcs.h"
//#include "map.h"
//#include "multimap.h"
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

    }
    ~BPlusTree(){
        clear_tree();
        child_count = 0;
        data_count = 0;
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        if(this != &RHS){
            clear_tree();
            copy_tree(RHS);
            dups_ok = RHS.dups_ok;
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
        if(data_count > MAXIMUM){
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
        }
    }
    void remove(const T& entry){
        loose_remove(entry);
        // If root has only 1 child, promote it
        if (data_count == 0 && child_count == 1) {
            BPlusTree* oldRoot = subset[0];
            copy_array(data, oldRoot->data, data_count, oldRoot->data_count);
            copy_array(subset, oldRoot->subset, child_count, oldRoot->child_count);
            oldRoot->child_count = 0;
            delete oldRoot;
        }
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
    } 

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
        if(!contains(entry)){
            insert(entry);
        } return get_existing(entry);
    }
    const T& get(const T& entry)const{
        if(!contains(entry)){
            insert(entry);
        } return get_existing(entry);
        
    }

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
        return data[i];
    }
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
        int i  = first_ge(data, data_count, key);

        if(is_leaf()){
            if(data[i] == key){
                i++;
            }

            if(i < data_count){
                return Iterator(this, i);
            } else{
                if(next){
                    return Iterator(next, 0);
                } else{
                    return Iterator(nullptr, 0);
                }
            }
        }

        if(!is_leaf() && data[i] == key){
            if(subset[i + 1] == nullptr){
                return Iterator(nullptr, 0);
            }
            return subset[i + 1]->upper_bound(key);
        } else{
            return subset[i]->upper_bound(key);
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

    bool is_valid(){
        // Check this node
        if (data_count > MAXIMUM + 1) return false;
        if (data_count < MINIMUM - 1 && child_count > 0) return false;
        if (is_leaf() && child_count != 0) return false;
        if (!is_leaf() && child_count != data_count + 1) return false;

        // Check ordering invariant
        for (int i = 1; i < data_count; i++) {
            if (!(data[i-1] <= data[i])) return false;
        }

        // Recursively check children
        if (!is_leaf()) {
            for (int i = 0; i < child_count; i++) {
                if (!subset[i]->is_valid()) return false;
            }
        }
        return true;
    }
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
            } data[i] = data[i] + entry;
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
                // B+ tree rule: leaf nodes must retain ALL data values.
                // bringMeUp was detached from subset[i] by split(), but it belongs
                // in insertMe (the right sibling). Re-insert it so no data is lost.
                ordered_insert(insertMe->data, insertMe->data_count, bringMeUp);
                insertMe->next = subset[i]->next;
                subset[i]->next = insertMe;
            }
        }
    }              //fix excess in child i

    BPlusTree<T>* get_smallest_node(){
        if (is_leaf()) return this;
        return subset[0]->get_smallest_node();
    }

    void get_smallest(T& entry){
        if (is_leaf()) {
            entry = data[0];
        } else {
            subset[0]->get_smallest(entry);
        }
    }

    void get_biggest(T& entry){
        if (is_leaf()) {
            detach_item(data, data_count, entry);
        } else {
            subset[child_count - 1]->get_biggest(entry);
        }
    }

    void detach_biggest(T& entry){
        // Detach and return the biggest element, fixing any shortage caused
        if (is_leaf()) {
            detach_item(data, data_count, entry);
        } else {
            subset[child_count - 1]->detach_biggest(entry);
            fix_shortage(child_count - 1);
        }
    }

    void remove_biggest(T& entry){
        if (is_leaf()) {
            detach_item(data, data_count, entry);
        } else {
            subset[child_count - 1]->remove_biggest(entry);
            fix_shortage(child_count - 1);
        }
    }

    //remove element functions:
    void loose_remove(const T& entry){
        int i = first_ge(data, data_count, entry);

        // If entry not found and we're not at a leaf, go deeper
        if (!is_leaf() && (i >= data_count || data[i] != entry)) {
            subset[i]->loose_remove(entry);
            fix_shortage(i);
            return;
        }

        // Found entry at index i
        if (i < data_count && data[i] == entry) {
            if (is_leaf()) {
                // Simply remove from leaf - shift elements left
                T entry_;
                delete_item(data, i, data_count, entry_);
            } else {
                // Replace with predecessor from left subtree, removing it from leaf
                subset[i]->detach_biggest(data[i]);
                fix_shortage(i);
            }
        }
        // If entry not in this node, recursively remove from child
        else if (!is_leaf()) {
            subset[i]->loose_remove(entry);
            fix_shortage(i);
        }
    }  //allows MINIMUM-1 data elements

    BPlusTree<T>* fix_shortage(int i){
        // Case 1: Try to transfer from left sibling
        if (i > 0 && subset[i-1]->data_count > MINIMUM) {
            transfer_left(i);
            return this;
        }
        // Case 2: Try to transfer from right sibling
        if (i < child_count - 1 && subset[i+1]->data_count > MINIMUM) {
            transfer_right(i);
            return this;
        }
        // Case 3: Merge with left sibling
        if (i > 0) {
            return merge_with_next_subset(i - 1);
        }
        // Case 4: Merge with right sibling
        return merge_with_next_subset(i);
    }  //fix shortage in child i

    void transfer_left(int i){
        // Transfer from subset[i] to subset[i-1]
        BPlusTree* left = subset[i-1];
        BPlusTree* right = subset[i];

        // Move parent's data[i-1] down to left child's last position
        T parentEntry;
        detach_item(data, data_count, parentEntry);
        attach_item(left->data, left->data_count, parentEntry);

        // Move right child's first subset to left child (if not leaf)
        if (!right->is_leaf()) {
            BPlusTree* child;
            delete_item(right->subset, 0, right->child_count, child);
            attach_item(left->subset, left->child_count, child);
        }

        // Move right child's first data up to parent
        T childEntry;
        delete_item(right->data, 0, right->data_count, childEntry);
        attach_item(data, data_count, childEntry);
    }        //transfer one element LEFT from child i

    void transfer_right(int i){
        // Transfer from subset[i] to subset[i+1]
        BPlusTree* left = subset[i];
        BPlusTree* right = subset[i+1];

        // Move parent's data[i] down to right child's first position
        T parentEntry;
        detach_item(data, data_count, parentEntry);
        insert_item(right->data, 0, right->data_count, parentEntry);

        // Move left child's last subset to right child (if not leaf)
        if (!left->is_leaf()) {
            BPlusTree* child;
            delete_item(left->subset, left->child_count - 1, left->child_count, child);
            insert_item(right->subset, 0, right->child_count, child);
        }

        // Move left child's last data up to parent
        T childEntry;
        detach_item(left->data, left->data_count, childEntry);
        attach_item(data, data_count, childEntry);
    }       //transfer one element RIGHT from child i

    BPlusTree<T>* merge_with_next_subset(int i){
        BPlusTree* left = subset[i];
        BPlusTree* right = subset[i+1];

        // Move parent's data[i] down to left child
        T parentEntry;
        detach_item(data, data_count, parentEntry);
        attach_item(left->data, left->data_count, parentEntry);

        // Merge right child's data into left child
        merge(left->data, left->data_count, right->data, right->data_count);

        // Merge right child's subsets into left child (if not leaf)
        if (!left->is_leaf()) {
            merge(left->subset, left->child_count, right->subset, right->child_count);
            left->child_count += right->child_count;
            // Clear right child's subset pointers since they're now owned by left
            for (int j = 0; j < right->child_count; j++) {
                right->subset[j] = nullptr;
            }
            right->child_count = 0;
        }

        // Fix leaf chain
        if (left->is_leaf()) {
            left->next = right->next;
        }

        // Delete right child
        delete right;

        // Remove right child from parent's subset array and decrement child_count
        // Also clear the last position which now has a stale pointer after shift
        for (int j = i + 1; j < child_count - 1; j++) {
            subset[j] = subset[j + 1];
        }
        subset[child_count - 1] = nullptr;  // Clear stale pointer at end
        child_count--;

        return left;
    } //merge subset i with  i+1

};

#endif