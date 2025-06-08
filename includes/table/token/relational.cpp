#include "relational.h"
using namespace std;

Relational::Relational(string value):TokenTable(1){
    _value = value;
}

string Relational::getValue(){
    return _value;
}

void Relational::print() const{
    cout << _value;
}