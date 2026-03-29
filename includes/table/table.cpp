#include "table.h"

Table::Table(){
    _table_name = "blank";
    _last_record = 0;
    _num_fields = 0;
    string textFileName = _table_name  + "_fields.txt";
    string binFileName = _table_name + ".bin";
    ofstream f2;
    f2.open(textFileName.c_str());
    f2.close();
    fstream f;
    open_fileW(f, binFileName.c_str());
    f.close();
}

Table::Table(string name, vector<string> fields){
    _indices.clear();
    _table_name = name;
    _field_names = fields;
    _last_record = 0;
    _num_fields = _field_names.size();

    string textFileName = _table_name  + "_fields.txt";
    string binFileName = _table_name + ".bin";

    ofstream f2;
    f2.open(textFileName.c_str());
    for(int i = 0; i < _num_fields; i++){
        f2 << _field_names[i] << endl;
    }
    f2.close();
    for(int i = 0; i < _num_fields; i++){
        _indices.push_back(MMap<string, long>());
    }
    fstream f;
    open_fileW(f, binFileName.c_str());
    f.close();
}

Table::Table(const string& tableName){
    fstream f;
    string binFileName = tableName + ".bin";
    string txtFileName = tableName + "_fields.txt";
    _table_name = tableName;
    _last_record = 0;
    f.open(txtFileName.c_str());
    string input;
    while(getline(f, input)){
        _field_names.push_back(input);
    }
    _num_fields = _field_names.size();
    f.close();

    for(int i = 0; i < _num_fields; i++){
        _indices.push_back(MMap<string, long>());
    }

    open_fileRW(f, binFileName.c_str());
    int i = 0;
    FileRecord r2;
    long bytes = r2.read(f, i);
    while(bytes>0){
        vector<string> row;
        for(int i = 0; i < _num_fields; i++){
            row.push_back(r2._record[i]);
        }
        insert_into_no_file_update(row);
        _last_record++;
        i++;
        bytes = r2.read(f, i);
    }

    f.close();
}

void Table::insert_into_no_file_update(const vector<string>& record){
    for(int i = 0; i < _field_names.size(); i++){
        _indices[i].insert(record[i], _last_record);
    }
}

void Table::insert_into(const vector<string>& record){    
    for(int i = 0; i < _field_names.size(); i++){
        if(record[i][0] == '\0'){
            break;
        }
        _indices[i].insert(record[i], _last_record);
    }            
    vector<string> copy = record;
    copy.push_back("\0");
    FileRecord r(copy);
    fstream f;
    string binFileName = _table_name + ".bin";
    open_fileRW(f, binFileName.c_str());
    r.write(f);
    _last_record++;
    f.close();
}

Table Table::select(vector<string> desiredFields, string p, string desiredOperator, string q){
    _records_PQ.clear();
    string desiredTableName;
    if(desiredOperator == ">"){
        desiredTableName = p + "GT" + q;
    } else if(desiredOperator == ">="){
        desiredTableName = p + "GTE" + q;
    } else if(desiredOperator == "<"){
        desiredTableName = p + "LT" + q;
    } else if(desiredOperator == "<="){
        desiredTableName = p + "LTE" + q;
    } else{
        desiredTableName = p + desiredOperator + q;
    }
    Table return_me(desiredTableName, desiredFields);
    int i = 0;
    while(i < _field_names.size() && p != _field_names[i]){
        i++;
    }
    vector<long> recordsPQ;

    if(desiredOperator == "="){
        //recordsPQ = _indices[i].at(q);
        MMap<string,long>::Iterator it = _indices[i].find(q);
        if(it != _indices[i].end()){
            recordsPQ = (*it).value_list;
        }
    } else if(desiredOperator == ">"){
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(desiredOperator == ">="){
        //recordsPQ = _indices[i].at(q);
        MMap<string,long>::Iterator it = _indices[i].find(q);
        if(it != _indices[i].end()){
            recordsPQ = (*it).value_list;
        }
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(desiredOperator == "<"){
        MMap<string, long>::Iterator start = _indices[i].begin();
        MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
        while(start != end){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(desiredOperator == "<="){
        MMap<string, long>::Iterator start = _indices[i].begin();
        MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
        end++;
        while(start != end){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    }

    _records_PQ = recordsPQ;
    
    fstream f;
    string tempFileName = _table_name + ".bin";
    open_fileRW(f, tempFileName.c_str());
    FileRecord r;
    for(int i = 0; i < recordsPQ.size(); i++){
        long bytes = r.read(f, recordsPQ[i]);
        if(bytes > 0){
            vector<string> recordToVector;
            
            for(int i = 0; i < desiredFields.size(); i++){
                int j = 0;
                while(desiredFields[i] != _field_names[j]){
                    j++;
                }
                recordToVector.push_back(r._record[j]);
            }
            return_me.insert_into(recordToVector);
        }
    }

    f.close();
    return return_me;
}

Table Table::select(vector<string> desiredFields, Queue<TokenTable *> postfix){
    string baseName = "tempPostFix";
    string newFileName = baseName;

    int c = 1;
    while (file_exists((newFileName + ".bin").c_str())) {
        newFileName = baseName + "(" + to_string(c) + ")";
        c++;
    }
    Table return_me(newFileName, desiredFields);

    _records_PQ.clear();
    Stack<TokenTable*> token_stack;
    token_stack = Stack<TokenTable *>();
    Stack<vector<long>> record_stack;
    record_stack = Stack<vector<long>>();
    while(!postfix.empty()){
        TokenTable* popped = postfix.pop();
        if(popped->getType() == 0){
            token_stack.push(popped);
        } else if(popped->getType() == 1){
            Relational* op = static_cast<Relational*>(popped);
            string desiredOperator = op->getValue();
            TokenStr* RHS = static_cast<TokenStr*>(token_stack.pop());
            TokenStr* LHS = static_cast<TokenStr*>(token_stack.pop());
            string p = LHS->getValue();
            string q = RHS->getValue();
            vector<long> potentialInsert = table_to_vector(p, desiredOperator, q);
            record_stack.push(potentialInsert);
            
        } else if(popped->getType() == 2){
            Logical* log = static_cast<Logical*>(popped);
            string desiredLogical = log->getValue();
            vector<long> RHS = record_stack.pop();
            vector<long> LHS = record_stack.pop();
            record_stack.push(applyLogical(LHS, RHS, desiredLogical));
        }
    }

    vector<long> endRecords = record_stack.pop();
    _records_PQ = endRecords;

    fstream f;
    string tempFileName = _table_name + ".bin";
    open_fileRW(f, tempFileName.c_str());
    FileRecord r;
    for(int i = 0; i < endRecords.size(); i++){
        long bytes = r.read(f, endRecords[i]);

        if(bytes > 0){
            vector<string> recordToVector;

            for(int i = 0; i < _field_names.size(); i++){
                recordToVector.push_back(r._record[i]);                     // ALL FIELDS
            }

            return_me.insert_into(recordToVector);
        }
    }

    return_me._records_PQ = endRecords;

    f.close();
    return return_me;

    
}

Table Table::select(vector<string> desiredFields, vector<string> infix){
    Queue<TokenTable *> postFix;
    postFix = Queue<TokenTable *>();
    Stack<TokenTable *> converter;
    converter = Stack<TokenTable *>();

    int parenTracker = 0;

    for(int i = 0; i < infix.size(); i++){
        string token = infix[i];

        if(token == "("){
            parenTracker++;
            converter.push(new TokenStr(token));
        }
        else if(token == ")"){
            parenTracker--;
            while(!converter.empty() && static_cast<TokenStr*>(converter.top())->getValue() != "("){
                postFix.push(converter.pop());
            }
            if(!converter.empty()){
                TokenTable* top = converter.pop();
                if(top){
                    delete top;
                } else{
                    assert(true && "should never get here");
                }
            }
        }
        else if(token == "and" || token == "or" || token == "=" || token == "<" || token == ">" || token == "<=" || token == ">="){
            while(!converter.empty()){
                TokenTable* top = converter.top();
                string topVal;

                int type = top->getType();
                if(type == 1){
                    topVal = static_cast<Relational*>(top)->getValue();
                } else if(type == 2){
                    topVal = static_cast<Logical*>(top)->getValue();
                } else if(type == 0){
                    topVal = static_cast<TokenStr*>(top)->getValue();
                }

                int topPriority;
                int tokenPriority;
                topPriority = getPriority(topVal);
                tokenPriority = getPriority(token);


                if(topPriority >= tokenPriority){
                    postFix.push(converter.pop());
                } else{
                    break;
                }
            }

            if(token == "and" || token == "or"){
                converter.push(new Logical(token));
            } else{
                converter.push(new Relational(token));
            }
        } else{
            postFix.push(new TokenStr(token));
        }
    }
    while(!converter.empty()){
        TokenTable* push_me = converter.pop();
        postFix.push(push_me);
    }
    return select(desiredFields, postFix);
}

int getPriority(const string& input){
    if(input == "or"){
        return 0;
    }
    if(input == "and"){ return 1;}
    if(input == "=" || input == "<" || input == ">" || input == "<=" || input == ">="){return 2;}
    return -1;
}

vector<long> Table::table_to_vector(string p, string op, string q){
    int i = 0;
    while(i < _field_names.size() && p != _field_names[i]){
        i++;
    }

    vector<long> recordsPQ;
    if(i == _field_names.size()){
        return recordsPQ;
    }

    if(op == "="){
        MMap<string,long>::Iterator it = _indices[i].find(q);
        if(it != _indices[i].end()){
            recordsPQ = (*it).value_list;
        }
        return recordsPQ;
    }

    else if(op == ">"){
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(op == "<"){
        MMap<string, long>::Iterator start = _indices[i].begin();
        MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
        while(start != end){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(op == ">="){
        MMap<string,long>::Iterator it = _indices[i].find(q);
        if(it != _indices[i].end()){
            recordsPQ = (*it).value_list;
        }
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(op == "<="){
        MMap<string, long>::Iterator start = _indices[i].begin();
        MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
        if(_indices[i].contains(q)){
            end++;
        }
        while(start != end){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    }
    return recordsPQ;
}

// Currently unimplemented.
Table Table::vectorToTable(vector<long> desiredRecords, string tName){
    return Table();
}

vector<long> Table::select_recnos(){
    return _records_PQ;
}

ostream &operator<<(ostream &outs, const Table &t){
    outs << "TABLE: " << t._table_name << endl;
    if(t._field_names.size() == 0){
        return outs;
    }

    for(int i = 0; i < t._field_names.size(); i++){
        outs << setw(25) << t._field_names[i];
    } 
    
    outs << endl;
    fstream f;
    FileRecord r2;
    string binName = t._table_name + ".bin";
    open_fileRW(f, binName.c_str());
    int i = 0;
    long bytes = r2.read(f, i);
    while(bytes > 0){
      outs << r2 << endl;
      i++;
      bytes = r2.read(f, i);
    }
    f.close();
    return outs;
}

vector<long> applyLogical(vector<long> LHS, vector<long> RHS, string logical){
    vector<long> result;

    sort(RHS.begin(), RHS.end());
    sort(LHS.begin(), LHS.end());
    if(logical == "and"){
        set_intersection(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
    } else if(logical == "or"){
        set_union(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
    }
    return result;
}

Table operator+(const Table& lhs, const Table& rhs){
    return Table(); // unused
}

Table Table::selectAll(vector<string> desiredFields){
    _records_PQ.clear();
    for(long i = 0; i < _last_record; i++){
        _records_PQ.push_back(i);
    }
    return *this;
}

vector<string> Table::getFields(){
    return _field_names;
}

void Table::update(string field, string value, vector<string> condition){
    _records_PQ.clear();
    select(_field_names, condition);
    vector<long> recordsToUpdate = _records_PQ;

    if(recordsToUpdate.empty()){
        return;
    }

    int fieldIndex = 0;
    while(fieldIndex < _field_names.size() && field != _field_names[fieldIndex]){
        fieldIndex++;
    }
    if(fieldIndex == _field_names.size()){
        return;
    }

    fstream f;
    string binFileName = _table_name + ".bin";
    open_fileRW(f, binFileName.c_str());
    FileRecord r;

    for(int i = 0; i < recordsToUpdate.size(); i++){
        long recno = recordsToUpdate[i];
        r.read(f, recno);

        strncpy(r._record[fieldIndex], value.c_str(), MAX);

        long pos = recno * sizeof(r._record);
        f.seekp(pos, ios_base::beg);
        f.write(&r._record[0][0], sizeof(r._record));
    }

    f.close();

    rebuild_index(fieldIndex);
}

void Table::updateAll(string field, string value){
    _records_PQ.clear();
    for(long i = 0; i < _last_record; i++){
        _records_PQ.push_back(i);
    }

    int fieldIndex = 0;
    while(fieldIndex < _field_names.size() && field != _field_names[fieldIndex]){
        fieldIndex++;
    }
    if(fieldIndex == _field_names.size()){
        return;
    }

    fstream f;
    string binFileName = _table_name + ".bin";
    open_fileRW(f, binFileName.c_str());
    FileRecord r;

    for(long recno = 0; recno < _last_record; recno++){
        r.read(f, recno);

        strncpy(r._record[fieldIndex], value.c_str(), MAX);

        long pos = recno * sizeof(r._record);
        f.seekp(pos, ios_base::beg);
        f.write(&r._record[0][0], sizeof(r._record));
    }

    f.close();

    rebuild_index(fieldIndex);
}

void Table::rebuild_index(int fieldIndex){
    _indices[fieldIndex].clear();
    fstream f;
    string binFileName = _table_name + ".bin";
    open_fileRW(f, binFileName.c_str());
    FileRecord r;
    for(long recno = 0; recno < _last_record; recno++){
        r.read(f, recno);
        if(r._record[fieldIndex][0] != '\0'){
            _indices[fieldIndex].insert(r._record[fieldIndex], recno);
        }
    }
    f.close();
}