#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <assert.h>
#include <cstdio>
#include <algorithm>

#include "../bplustree/btree_array_funcs.h"
#include "../bplustree/bplustree.h"
#include "../bplustree/multimap.h"
#include "file_record.h"
#include "../queue/MyQueue.h"
#include "token/token.h"
#include "token/tokenstr.h"
#include "token/relational.h"
#include "token/logical.h"
#include "../stack/MyStack.h"

using namespace std;
using vectorstr = vector<string>;


class Table
{
public:
    Table();
        //_field_names = field_names;
    Table(string name, vector<string> fields);
    Table(const string& tableName);
    void insert_into(const vector<string>& record);
    void insert_into_no_file_update(const vector<string>& record);
    Table select(vector<string> desiredFields, string p, string desiredOperator, string q);
    Table select(vector<string> desiredFields, Queue<TokenTable*> postfix);
    Table select(vector<string> desiredFields, vector<string> infix);
    Table selectAll(vector<string> desiredFields);
    vector<long> table_to_vector(string p, string op, string q);
    Table vectorToTable(vector<long> desiredRecords, string tName);
    vector<long> select_recnos();
    friend Table operator+(const Table& lhs, const Table& rhs);
    friend ostream &operator <<(ostream& outs, const Table &t);
    
    //~Table();

//Table select(vector search, vector wanted)

private:
    //vector<string> _field_names;
    string _table_name;
    vector<string> _field_names;
    vector<long> _records_PQ;
    int _num_fields;
    vector<MMap<string, long>> _indices;
    long _last_record;
    //fstream f;

};

vector<long> applyLogical(vector<long> LHS, vector<long> RHS, string logical);
int getPriority(const string& input);


#endif