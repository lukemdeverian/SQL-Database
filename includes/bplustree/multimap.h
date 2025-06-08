#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <vector>
#include <iostream>
#include "bplustree.h"
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
    }
    MPair(const K& k, const V& v){
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs << print_me.key << ", " << print_me.value_list;
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return !(lhs.key == rhs.key);
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key < rhs.key);
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key <= rhs.key);
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return !(lhs < rhs);
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        vector<V> combined = lhs.value_list;
        for(int i = 0; i < rhs.value_list.size(); i++){
            combined.push_back(rhs.value_list[i]);
        }
        return MPair<K, V>(lhs.key, combined);
    }
};


template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it);
        Iterator(){
            _it = nullptr;
        }
        Iterator operator ++(int unused){
            Iterator temp = *this;
            _it++;
            return temp;
        }
        Iterator operator ++(){
            //return (_it++);
            ++_it;
            return *this;
        }
        MPair<K, V> operator *(){
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs._it == rhs._it);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs._it == rhs._it);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap(){

    }

//  Iterators
    Iterator begin(){
        return Iterator(mmap.begin());
    }
    Iterator end(){
        return Iterator(mmap.end());
    }

//  Capacity
    int size() const{
        return mmap.size();
    }
    bool empty() const{
        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const{
        return mmap.get(key).value_list;
    }
    vector<V>& operator[](const K& key){
        return mmap.get(key).value_list;
    }

    vector<V>& at(const K& key){
        //cout << "PRINTING FROM AT: " << mmap.get(key).value_list << endl;
        return mmap.get(key).value_list;
    }
    const vector<V>& at(const K& key) const{
        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){
        MPair<K, V> insertMe(k, v);
        //cout << "insert me: " << insertMe << endl;
        mmap.insert(insertMe);
    }
    void erase(const K& key);
    void clear(){
        mmap.clear_tree();
    }

    void print_lookup(){
        if(empty()){
            return;
        }

        cout << left << setw(15) << "KEY" << "VALUE(S)" << endl;
        cout << string(30, '-') << endl;

        MMap<string, string>::Iterator it;

        for(it = begin(); it != end(); it++){
            MPair<string, string> entry = *it;
            const string& key = entry.key;
            const vector<string>& values = entry.value_list;

            cout << left << setw(15) << key;

            for(int i = 0; i < values.size(); i++){
                cout << values[i];
                if(i != values.size() - 1){
                    cout << " ";
                }
            }

            cout << endl;
        }
    }

//  Operations:
    bool contains(const K& key) const{
        return mmap.contains(key);
    }
    vector<V> &get(const K& key){
        return mmap.get(key).value_list;
    }

    Iterator find(const K& key){
        return Iterator(mmap.find(key));
    }
    int count(const K& key){
        return mmap.size();
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(const K& key){
        return Iterator(mmap.lower_bound(key));
    }

    Iterator upper_bound(const K& key){
        return Iterator(mmap.upper_bound(key));
    }



    bool is_valid(){
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

template <typename K, typename V>
MMap<K, V>::Iterator::Iterator(typename MMap<K, V>::map_base::Iterator it)    : _it(it) {}


template <typename T>
vector<T>& operator+=(vector<T>& v, const T& item) {
    v.push_back(item);
    return v;
}

template <typename T>
ostream& operator<<(std::ostream& outs, const vector<T>& vec){
    outs << "[";
    for(size_t i = 0; i < vec.size(); ++i){
        outs << vec[i];
        if(i != vec.size() - 1){
            outs << ", ";
        }
    }
    outs << "]";
    return outs;
}

#endif