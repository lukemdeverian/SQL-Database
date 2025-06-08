#include "logical.h"
using namespace std;

Logical::Logical(string value):TokenTable(2){
    _value = value;
}

void Logical::print() const{
    cout << _value;
}

string Logical::getValue(){
    return _value;
}