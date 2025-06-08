#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <cstring>
#include "constants.h"
using namespace std;

class Token
{
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type() const;
    string type_string() const;
    string token_str() const;
    void reset_token();
    void append(char c);
    void append(string s);
    void setType(int type);
    void removeEnd();
private:
    string _token;
    int _type;
};

#endif