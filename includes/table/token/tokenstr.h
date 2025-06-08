#ifndef TOKENSTR_H
#define TOKENSTR_H
#include "token.h"

class TokenStr: public TokenTable{
    public:
        TokenStr(string value);
        string getValue();
        void print() const;
    private:
        string _value;
};

#endif