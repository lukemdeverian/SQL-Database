#include "token.h"
using namespace std;

TokenTable::TokenTable(){
    _type = 0;
}

TokenTable::TokenTable(int type){
    _type = type;
}

int TokenTable::getType(){
    return _type;
}

void TokenTable::print() const{
    cout << _type;
}

ostream& operator <<(ostream& outs, const TokenTable &print_me){
    print_me.print();
    return outs;
}