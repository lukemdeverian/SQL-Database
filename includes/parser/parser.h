#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <assert.h>

#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
#include "../table/typedefs.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../tokenizer/stokenize.h"

const int MAX_PROWS = 40;
const int MAX_PCOLS = 40;


using namespace std;

using mmap_ss = MMap<string, string>;

class Parser{
    public:
        Parser();
        Parser(char s[]);
        mmap_ss parse_tree();
        void print_lookup();
        void set_string(char s[]);

    private:
        mmap_ss _ptree;
        int _ptable[MAX_PROWS][MAX_PCOLS];
        Map<string, int> keywords;
        Queue<string> tokenized;
        void make_ptable();
        void makeKeywordsMap();
        Queue<string> setInput(char s[]);
};


#endif