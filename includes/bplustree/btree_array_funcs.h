#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H
#include <iostream>
#include <vector>
using namespace std;

template <class T>
T maximal(const T& a, const T& b){
    if(a < b){
        return b;
    } return a;
}                      //return the larger of the two items

template <class T>
void my_swap(T& a, T& b){
    T placeHolder = a;
    a = b;
    b = placeHolder;
}  //my_swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    int indexOfMax = 0;
    for(int i = 1; i < n; i++){
        if(data[i] > data[indexOfMax]){
            indexOfMax = i;
        }
    } return indexOfMax;
}                 //return index of the largest item in data

template <class T>
int first_ge(const T data[ ], int n, const T& entry){
    for(int i = 0; i < n; i++){
        if(entry <= data[i]){
            return i;
        }
    } return n;
}   //return the first element in data that is

template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    for(int i = 0; i < n; i++) {
        if (data[i] == entry) {
            return;
        }
    }

    int insert_pos = first_ge(data, n, entry);

    for(int i = n; i > insert_pos; --i){
        data[i] = data[i - 1];
    }

    data[insert_pos] = entry;
    n++;
}        //insert entry into the sorted array
                                                        //data with length n
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n] = entry;
    n++;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    for (int j = n; j > i; j--) {
        data[j] = data[j - 1];
    }
    data[i] = entry;
    n++;
}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    // data[n-1] = entry;
    // n--;
    if(n < 0){
        cout << "TERRIBLE DETACH ITEM!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    n--;
    entry = data[n];
}          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    entry = data[i];
    for (int j = i; j < n - 1; j++) {
        data[j] = data[j + 1];
    }
    n--;
}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    int j = 0;
    for(int i = n1; i < n1+n2; i++){
        data1[i] = data2[j];
        j++;
    }
    n1 += n2;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    // if(n1 % 2 != 0){
    //     cout << "ODD\n";
    // } else {
    //     cout << "EVEN\n";
    // }
    //cout << "BEFORE N1: " << n1 << endl;

    // int iterations = n1/2;
    // int j = 0;
    // if(iterations % 2 == 0){
    //     for(int i = iterations; i < n1; i++){
    //         data2[j] = data1[i];
    //         j++;
    //     }
    //     n2 = j;
    //     n1 = iterations;
    // }
    // else{
    //     for(int i = iterations + 1; i < n1; i++){
    //         data2[j++] = data1[i];
    //     }
    //     n2 = j;
    //     n1 = iterations + 1;
    // }

    n2 = n1/2;
    for(int i = n1-n2; i < n1; i++){
        data2[i - (n1-n2)] = data1[i];
    }
    n1 = n1-n2;
}




    

    //cout << "N1 + N2: " << n1 + n2 << endl;
    
   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    dest_size = src_size;
    for(int i = 0; i < dest_size; i++){
        dest[i] = src[i];
    }
}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1){
    for(int i = 0; i < n; i++){
        if(i == pos){
            cout << "[" << data[i] << "] ";
        } else{
            cout << data[i] << " ";
        }
    }
}  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(item <= data[i]){
            return false;
        }
    } return true;
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(item > data[i]){
            return false;
        }
    } return true;
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme


#endif