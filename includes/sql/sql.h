#ifndef SQL_H
#define SQL_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <assert.h>

#include "../table/table.h"
#include "../parser/parser.h"

using namespace std;
using mmap_ss = MMap<string, string>;
using vectorstr = vector<string>;
using vectorlong = vector<long>;

class SQL{
    public:
        SQL();
        Table command(string theCommand);
        vector<long> select_recnos();
    private:
        mmap_ss _ptree;
        vector<long> _records_selected;
        Map<string, Table> _tables;
};

#endif