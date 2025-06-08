#ifndef TOKENTABLE_H
#define TOKENTABLE_H
#include <string>
#include <iostream>
using namespace std;
//#include "tokenstr.h"
//#include "relational.h"

class TokenTable{
    public:
        TokenTable();
        TokenTable(int type);
        int getType();
        virtual void print() const;
        friend ostream& operator <<(ostream& outs, const TokenTable &print_me);
    private:
        int _type;
};

#endif