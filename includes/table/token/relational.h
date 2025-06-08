#ifndef RELATIONAL_H
#define RELATIONAL_H
#include "token.h"

class Relational: public TokenTable{
    public:
        Relational(string value);
        string getValue();
        void print() const;
    private:
        string _value;
};

#endif