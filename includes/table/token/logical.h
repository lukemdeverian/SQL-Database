#ifndef LOGICAL_H
#define LOGICAL_H
#include "token.h"

class Logical: public TokenTable{
    public:
        Logical(string value);
        string getValue();
        void print() const;
    private:
        string _value;
};

#endif