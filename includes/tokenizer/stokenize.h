#ifndef STOKENIZE_H
#define STOKENIZE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include "token.h"
#include "constants.h"
using namespace std;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char input[]);
    bool get_token(Token &token);
    bool done();
    bool more();
    int get_postion();
    void setBuffer(char buff[]);
    void set_string(char buff[]);
    friend STokenizer& operator >> (STokenizer &STK, Token &token);
private:
    int table[MAX_ROWS][MAX_COLUMNS];
    int pos;
    char buffer[MAX_BUFFER];
    bool _done;
};

#endif