#include "tokenstr.h"
using namespace std;

TokenStr::TokenStr(string value):TokenTable(0){
    _value = value;
}

void TokenStr::print() const{
    cout << _value;
}

string TokenStr::getValue(){
    return _value;
}