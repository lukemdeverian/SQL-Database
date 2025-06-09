// // #include "table.h"

// // Table::Table(){

// // }

// // Table::Table(string name, vector<string> fields){
// //     _indices.clear();
// //     _table_name = name;
// //     _field_names = fields;
// //     _last_record = 0;
// //     _num_fields = _field_names.size();

// //     string textFileName = _table_name  + "_fields.txt";
// //     string binFileName = _table_name + ".bin";
// //     // remove(textFileName.c_str());
// //     // remove(binFileName.c_str());


// //     ofstream f2;
// //     f2.open(textFileName.c_str());
// //     //f2 << _num_fields << endl;
// //     for(int i = 0; i < _num_fields; i++){
// //         f2 << _field_names[i] << endl;
// //     }
// //     f2.close();
// //     for(int i = 0; i < _num_fields; i++){
// //         _indices.push_back(MMap<string, long>());
// //     }
// //     fstream f;
// //     open_fileW(f, binFileName.c_str());
// //     f.close();
// // }

// // Table::Table(const string& tableName){
// //         //cout << "did i crash?";

// //     fstream f;
// //     string binFileName = tableName + ".bin";
// //     string txtFileName = tableName + "_fields.txt";
// //     _table_name = tableName;
// //     _last_record = 0;
// //     f.open(txtFileName.c_str());
// //     string input;
// //     // if(getline(f, input)){
// //     //     _num_fields = stoi(input);
// //     // }
// //     // for(int i = 0; i < _num_fields; i++){
// //     //     getline(f, input);
// //     //     cout << "input: " << input << endl;
// //     //     _field_names.push_back(input);
// //     // }
// //     while(getline(f, input)){
// //         //cout << "input: " << input << endl;
// //         _field_names.push_back(input);
// //     }
// //     _num_fields = _field_names.size();
// //     //cout << "FIELD NAMES FROM STRING CONSTRUCTOR: " << _field_names << endl;
// //     f.close();

// //     for(int i = 0; i < _num_fields; i++){
// //         _indices.push_back(MMap<string, long>());
// //     }

// //     open_fileRW(f, binFileName.c_str());
// //     int i = 0;
// //     FileRecord r2;
// //     long bytes = r2.read(f, i);
// //     while(bytes>0){
// //         vector<string> row;
// //         for(int i = 0; i < _num_fields; i++){
// //             row.push_back(r2._record[i]);
// //         }
// //         insert_into_no_file_update(row);
// //         _last_record++;
// //         i++;
// //         bytes = r2.read(f, i);
// //     }

// //     f.close();
// // }

// // void Table::insert_into_no_file_update(const vector<string>& record){
// //     //cout << _indices[1] << endl;
// //     for(int i = 0; i < _num_fields; i++){
// //         _indices[i].insert(record[i], _last_record);
// //     }
// // }

// // void Table::insert_into(const vector<string>& record){
// //     //cout << "start of insert_into" << endl;
    
// //     for(int i = 0; i < _num_fields; i++){
// //         if(record[i][0] == '\0'){
// //             break;
// //         }
// //         _indices[i].insert(record[i], _last_record);
// //         //cout << _indices[i];
// //     }
// //     //cout << _indices[1] << endl;
// //     // cout << "\n\n\n\n";
// //     //     cout << _indices[2];
// //     // cout << "\n\n\n\n";
// //     //record.push_back("\0");               
// //     vector<string> copy = record;
// //     copy.push_back("\0");                        //potential issue?
// //     FileRecord r(copy);
// //     fstream f;
// //     string binFileName = _table_name + ".bin";
// //     open_fileRW(f, binFileName.c_str());
// //     r.write(f);
// //     _last_record++;
// //     f.close();
// // }

// // Table Table::select(vector<string> desiredFields, string p, string desiredOperator, string q){
// //     //cout << "DESIREDFIELDS: " << desiredFields << endl;

// //     _records_PQ.clear();
// //     string desiredTableName;
// //     if(desiredOperator == ">"){
// //         desiredTableName = p + "GT" + q;
// //     } else if(desiredOperator == ">="){
// //         desiredTableName = p + "GTE" + q;
// //     } else if(desiredOperator == "<"){
// //         desiredTableName = p + "LT" + q;
// //     } else if(desiredOperator == "<="){
// //         desiredTableName = p + "LTE" + q;
// //     } else{
// //         desiredTableName = p + desiredOperator + q;
// //     }
// //     Table return_me(desiredTableName, desiredFields);
// //     //cout << "RETURN ME FIELD NAMES: " << return_me._field_names << endl;
// //     int i = 0;
// //     while(i < _field_names.size() && p != _field_names[i]){
// //         i++;
// //     }
// //     //cout << "i: " << i << " num fields: " << _num_fields << "_fields: " << _field_names << endl;
// //     assert(i != _field_names.size() && "field was not found.");

// //     vector<long> recordsPQ;

// //     if(desiredOperator == "="){
// //         recordsPQ = _indices[i].at(q);
// //     } else if(desiredOperator == ">"){
// //         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
// //         while(start != _indices[i].end()){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(desiredOperator == ">="){
// //         recordsPQ = _indices[i].at(q);
// //         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
// //         while(start != _indices[i].end()){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(desiredOperator == "<"){
// //         MMap<string, long>::Iterator start = _indices[i].begin();
// //         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
// //         while(start != end){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(desiredOperator == "<="){
// //         MMap<string, long>::Iterator start = _indices[i].begin();
// //         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
// //         end++;
// //         while(start != end){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     }

// //     _records_PQ = recordsPQ;
    
// //     //cout << "recordspq: " << _records_PQ << endl;

// //     fstream f;
// //     string tempFileName = _table_name + ".bin";
// //     open_fileRW(f, tempFileName.c_str());
// //     FileRecord r;
// //     for(int i = 0; i < recordsPQ.size(); i++){
// //         long bytes = r.read(f, recordsPQ[i]);
// //         if(bytes > 0){
// //             vector<string> recordToVector;
            
// //             for(int i = 0; i < desiredFields.size(); i++){
// //                 int j = 0;
// //                 while(desiredFields[i] != _field_names[j]){
// //                     j++;
// //                 }
// //                 recordToVector.push_back(r._record[j]);
// //                 //recordToVector.push_back(r._record[i]);

// //             }
// //             return_me.insert_into(recordToVector);
// //         }
// //     }

// //     f.close();
// //     //cout << return_me._indices[0];
// //     //swap(return_me._indices[0], return_me._indices[2]);
// //     //cout << return_me._indices[0];                          //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
// //     //swap(return_me._indices[0], return_me._indices[1]);
// //     return return_me;
// // }

// // Table Table::select(vector<string> desiredFields, Queue<Token *> postfix){
// //     // cout << "\nFIELD NAMES: " << _field_names << endl;
// //     // cout << "DESIRED FIELDS: " << desiredFields << endl;
// //     // cout << "NUM OF FIELDS: " << _num_fields << endl << endl;
// //     // string newFileName = "tempPostFix";
// //     // int c = 1;
// //     // while(file_exists(newFileName.c_str())){
// //     //     newFileName = "tempPostFix(" + to_string(c) + ")";
// //     //     c++;
// //     // }
// //     string baseName = "tempPostFix";
// //     string newFileName = baseName;

// //     newFileName = _table_name;

// //     // int c = 1;
// //     // while (file_exists((newFileName + ".bin").c_str())) {
// //     //     newFileName = baseName + "(" + to_string(c) + ")";
// //     //     c++;
// //     // }
// //     //cout << "NEWFILENAME: " << newFileName << endl;
// //     Table return_me(newFileName, desiredFields);
    
    
// //     //return return_me;   //testing stub


// //     _records_PQ.clear();
// //     Stack<Token*> token_stack;
// //     token_stack = Stack<Token *>();
// //     Stack<vector<long>> record_stack;
// //     record_stack = Stack<vector<long>>();

// //     vector<long> inds;
// //     int j = 0;
// //     for(int i = 0; i < _num_fields; i++){
// //         if(j < desiredFields.size() && _field_names[i] == desiredFields[j]){
// //             inds.push_back(i);
// //             j++;
// //         }
// //     }

// //     //cout << "INDS: " << inds << endl;
// //     //int iterations = 0;
// //     while(!postfix.empty()){
// //         //cout << "ITERATION: " << iterations << ", ";
// //         Token* popped = postfix.pop();
// //         //cout << "popped: "; popped->print(); cout << endl;
// //         if(popped->getType() == 0){
// //             //cout << "type = 0 at popped = "; popped->print(); cout << endl;
// //             token_stack.push(popped);
// //         } else if(popped->getType() == 1){
// //             //cout << "type = 1 at popped = "; popped->print(); cout << endl;
// //             Relational* op = static_cast<Relational*>(popped);
// //             string desiredOperator = op->getValue();
// //             TokenStr* RHS = static_cast<TokenStr*>(token_stack.pop());
// //             TokenStr* LHS = static_cast<TokenStr*>(token_stack.pop());
// //             string p = LHS->getValue();
// //             string q = RHS->getValue();
// //             record_stack.push(table_to_vector(p, desiredOperator, q));
// //         } else if(popped->getType() == 2){
// //             Logical* log = static_cast<Logical*>(popped);
// //             string desiredLogical = log->getValue();
// //             vector<long> RHS = record_stack.pop();
// //             vector<long> LHS = record_stack.pop();
// //             record_stack.push(applyLogical(LHS, RHS, desiredLogical));
// //         }
// //         //iterations++;
// //     }

// //     vector<long> endRecords = record_stack.pop();
// //     _records_PQ = endRecords;

// //     fstream f;
// //     string tempFileName = _table_name + ".bin";
// //     open_fileRW(f, tempFileName.c_str());
// //     FileRecord r;
// //     //cout << "END RECORDS: " << endRecords << endl;
// //     //cout << "INDS: " << inds << endl;
// //     for(int i = 0; i < endRecords.size(); i++){
// //         long bytes = r.read(f, endRecords[i]);

// //         if(bytes > 0){
// //             //cout << "POTENTIAL RECORD: " << r << endl;
// //             vector<string> recordToVector;
// //             // for(int i = 0; i < inds.size(); i++){
// //             //     int j = 0;
// //             //     while(desiredFields[i] != _field_names[j]){                  //this might be an issue?
// //             //         j++;
// //             //     }
// //             //     //cout << "r.record[j]: " << r._record[j] << endl;                      //CHANGING FOR LOOP TO INCLUDE ALL FIELDS
// //             //     recordToVector.push_back(r._record[j]);
// //             //     // recordToVector.push_back(r._record[i]);
// //             // }

// //             for(int i = 0; i < _field_names.size(); i++){
// //                 recordToVector.push_back(r._record[i]);                     //ALL FIELDS
// //             }

// //             //cout << "before recordToVector: " << recordToVector << endl;
// //             //recordToVector.push_back("\0");
// //             //cout << "after recordToVector: " << recordToVector << endl;
// //             return_me.insert_into(recordToVector);
// //         }
// //     }

// //     f.close(); //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
// //     return return_me;

    
// // }

// // Table Table::select(vector<string> desiredFields, vector<string> infix){
// //     Queue<Token *> postFix;
// //     postFix = Queue<Token *>();
// //     Stack<Token *> converter;
// //     converter = Stack<Token *>();

// //     // postFix.push(new TokenStr("age"));
// //     // postFix.push(new TokenStr("20"));
// //     // postFix.push(new Relational("<"));
// //     // postFix.push(new TokenStr("age"));
// //     // postFix.push(new TokenStr("17"));
// //     // postFix.push(new Relational(">"));
// //     // postFix.push(new Logical("and"));
// //     // postFix.push(new TokenStr("lname"));
// //     // postFix.push(new TokenStr("Gomez"));
// //     // postFix.push(new Relational("="));
// //     // postFix.push(new Logical("or"));

// //     //return select(desiredFields, postFix);


// //     int parenTracker = 0;

// //     for(int i = 0; i < infix.size(); i++){
// //         string token = infix[i];

// //         if(token == "("){
// //             parenTracker++;
// //             converter.push(new TokenStr(token));
// //         }
// //         else if(token == ")"){
// //             parenTracker--;
// //             while(!converter.empty() && static_cast<TokenStr*>(converter.top())->getValue() != "("){
// //                 postFix.push(converter.pop());
// //             }
// //             if(!converter.empty()){
// //                 Token* top = converter.pop();
// //                 if(top){
// //                     delete top;
// //                 } else{
// //                     assert(true && "should never get here");
// //                 }
// //                 //delete converter.pop();  // discard "(" and free memory
// //             }
// //         }
// //         else if(token == "and" || token == "or" || token == "=" || token == "<" || token == ">" || token == "<=" || token == ">="){
// //             while(!converter.empty()){
// //                 Token* top = converter.top();
// //                 string topVal;

// //                 int type = top->getType();
// //                 if(type == 1){
// //                     topVal = static_cast<Relational*>(top)->getValue();
// //                 } else if(type == 2){
// //                     topVal = static_cast<Logical*>(top)->getValue();
// //                 } else if(type == 0){
// //                     topVal = static_cast<TokenStr*>(top)->getValue();
// //                 }

// //                 int topPriority;
// //                 int tokenPriority;
// //                 topPriority = getPriority(topVal);
// //                 tokenPriority = getPriority(token);


// //                 if(topPriority >= tokenPriority){
// //                     postFix.push(converter.pop());
// //                 } else{
// //                     break;
// //                 }
// //             }

// //             if(token == "and" || token == "or"){
// //                 converter.push(new Logical(token));
// //             } else{
// //                 converter.push(new Relational(token));
// //             }
// //         } else{
// //             postFix.push(new TokenStr(token));
// //         }
// //     }

// //     assert(parenTracker == 0 && "uneven parenthesis");

// //     while(!converter.empty()){
// //         Token* push_me = converter.pop();
// //         if(!push_me){
// //             cout << "nullptr in stack";
// //         }
// //         postFix.push(push_me);
// //     }

// //     // 15
// //     // Token* test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     //cout << "age 30 > lname Doe = and age 18 > fname Flo = and or";
// //     // test = postFix.pop();
// //     // test->print(); cout << " ";
// //     //cout << "\n\n\n\n";

// //     Queue<Token *> temp = postFix;
// //     while(!temp.empty()){
// //         temp.pop()->print(); cout << " ";
// //     }
    

// //     return select(desiredFields, postFix);
// // }

// // int getPriority(const string& input){
// //     if(input == "and" || input == "or"){ return 1;}
// //     if(input == "=" || input == "<" || input == ">" || input == "<=" || input == ">="){return 2;}
// //     return 0;
// // }

// // vector<long> Table::table_to_vector(string p, string op, string q){
// //     // cout << "\n\n\nFIRED TABLE_TO_VECTOR WITH: " << endl;
// //     // //cout << "DESIREDFIELDS: " << desiredFields << endl;
// //     // cout << "P: " << p << endl;
// //     // cout << "OP: " << op << endl;
// //     // cout << "Q: " << q << endl;
// //     int i = 0;
// //     while(i < _field_names.size() && p != _field_names[i]){
// //         i++;
// //     }
// //     //assert(i != _field_names.size() && "field was not found.");

// //     vector<long> recordsPQ;

// //     if(op == "="){
// //         recordsPQ = _indices[i].at(q);
// //         //cout << "\n\n\n\n       RECORDSPQ: " << recordsPQ << "\n\n\n\n";
// //     } else if(op == ">"){
// //         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
// //         while(start != _indices[i].end()){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(op == "<"){
// //         MMap<string, long>::Iterator start = _indices[i].begin();
// //         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
// //         while(start != end){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(op == ">="){
// //         recordsPQ = _indices[i].at(q);
// //         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
// //         while(start != _indices[i].end()){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     } else if(op == "<="){
// //         //cout << "HELLLLLLOOOOOO" << endl;
// //         MMap<string, long>::Iterator start = _indices[i].begin();
// //         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
// //         end++;
// //         while(start != end){
// //             for(int i = 0; i < (*start).value_list.size(); i++){
// //                 recordsPQ.push_back((*start).value_list[i]);
// //             }
// //             start++;
// //         }
// //     }
// //     //cout << "RECORDS PQ: " << recordsPQ << endl;
// //     return recordsPQ;
// // }

// // Table Table::vectorToTable(vector<long> desiredRecords, string tName){
// //     // fstream f;
// //     // open_fileRW(f, _table_name.c_str() + '.bin');
// //     // FileRecord r;


// //     return Table();
// // }

// // vector<long> Table::select_recnos(){
// //     return _records_PQ;
// // }

// // ostream &operator<<(ostream &outs, const Table &t){
// //     outs << "TABLE: " << t._table_name << endl;
// //     //outs << "num of fields: " << t._num_fields << endl;
// //     if(t._num_fields == 0){
// //         return outs;
// //     } //outs << t._field_names[0];
// //     for(int i = 0; i < t._num_fields; i++){
// //         outs << setw(25) << t._field_names[i];
// //     } 
    
// //     outs << endl;
// //     fstream f;
// //     FileRecord r2;
// //     string binName = t._table_name + ".bin";
// //     open_fileRW(f, binName.c_str());
// //     int i = 0;
// //     long bytes = r2.read(f, i);
// //     while(bytes > 0){
// //       outs << r2 << endl;
// //       i++;
// //       bytes = r2.read(f, i);
// //     }
// //     f.close();
// //     return outs;
// // }

// // vector<long> applyLogical(vector<long> LHS, vector<long> RHS, string logical){
// //     vector<long> result;

// //     sort(RHS.begin(), RHS.end());
// //     sort(LHS.begin(), LHS.end());
// //     //vector<long> result;
// //     if(logical == "and"){
// //         set_intersection(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
// //     } else if(logical == "or"){
// //         set_union(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
// //     }

// //     // if(logical == "or"){
// //     //     for(int i = 0; i < LHS.size(); i++){
// //     //         bool exists = false;
// //     //         for(int j = 0; j < result.size(); j++){
// //     //             if(result[j] == LHS[i]){
// //     //                 exists = true;
// //     //                 break;
// //     //             }
// //     //         }
// //     //         if(!exists){
// //     //             result.push_back(LHS[i]);
// //     //         }
// //     //     }

// //     //     for(int i = 0; i < RHS.size(); i++){
// //     //         bool exists = false;
// //     //         for(int j = 0; j < result.size(); j++){
// //     //             if(result[j] == RHS[i]){
// //     //                 exists = true;
// //     //                 break;
// //     //             }
// //     //         }
// //     //         if(!exists){
// //     //             result.push_back(RHS[i]);
// //     //         }
// //     //     }
// //     // } else if(logical == "and"){
// //     //     for(int i = 0; i < LHS.size(); i++){
// //     //         bool in_rhs = false;
// //     //         for(int j = 0; j < RHS.size(); j++){
// //     //             if(LHS[i] == RHS[j]){
// //     //                 in_rhs = true;
// //     //                 break;
// //     //             }
// //     //         }

// //     //         if(in_rhs){
// //     //             bool already_in = false;
// //     //             for(int k = 0; k < result.size(); k++){
// //     //                 if(result[k] == LHS[i]){
// //     //                     already_in = true;
// //     //                     break;
// //     //                 }
// //     //             }
// //     //             if(!already_in){
// //     //                 result.push_back(LHS[i]);
// //     //             }
// //     //         }
// //     //     }
// //     // }

// //     return result;
// // }


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// //                         the issues for it to not pass the grader couldve been the name of the file or the printing out specific fields

// #include "table.h"
// #include "sql.h"

// Table::Table(){

// }

// Table::Table(string name, vector<string> fields){
//     _indices.clear();
//     _table_name = name;
//     _field_names = fields;
//     _last_record = 0;
//     _num_fields = _field_names.size();

//     string textFileName = _table_name  + "_fields.txt";
//     string binFileName = _table_name + ".bin";
//     // remove(textFileName.c_str());
//     // remove(binFileName.c_str());


//     ofstream f2;
//     f2.open(textFileName.c_str());
//     //f2 << _num_fields << endl;
//     for(int i = 0; i < _num_fields; i++){
//         f2 << _field_names[i] << endl;
//     }
//     f2.close();
//     for(int i = 0; i < _num_fields; i++){
//         _indices.push_back(MMap<string, long>());
//     }
//     fstream f;
//     open_fileW(f, binFileName.c_str());
//     f.close();
// }

// Table::Table(const string& tableName){
//         //cout << "did i crash?";

//     fstream f;
//     string binFileName = tableName + ".bin";
//     string txtFileName = tableName + "_fields.txt";
//     _table_name = tableName;
//     _last_record = 0;
//     f.open(txtFileName.c_str());
//     string input;
//     // if(getline(f, input)){
//     //     _num_fields = stoi(input);
//     // }
//     // for(int i = 0; i < _num_fields; i++){
//     //     getline(f, input);
//     //     cout << "input: " << input << endl;
//     //     _field_names.push_back(input);
//     // }
//     while(getline(f, input)){
//         //cout << "input: " << input << endl;
//         _field_names.push_back(input);
//     }
//     _num_fields = _field_names.size();
//     //cout << "FIELD NAMES FROM STRING CONSTRUCTOR: " << _field_names << endl;
//     f.close();

//     for(int i = 0; i < _num_fields; i++){
//         _indices.push_back(MMap<string, long>());
//     }

//     open_fileRW(f, binFileName.c_str());
//     int i = 0;
//     FileRecord r2;
//     long bytes = r2.read(f, i);
//     while(bytes>0){
//         vector<string> row;
//         for(int i = 0; i < _num_fields; i++){
//             row.push_back(r2._record[i]);
//         }
//         insert_into_no_file_update(row);
//         _last_record++;
//         i++;
//         bytes = r2.read(f, i);
//     }

//     f.close();
// }

// void Table::insert_into_no_file_update(const vector<string>& record){
//     //cout << _indices[1] << endl;
//     for(int i = 0; i < _num_fields; i++){
//         _indices[i].insert(record[i], _last_record);
//     }
// }

// void Table::insert_into(const vector<string>& record){
//     //cout << "start of insert_into" << endl;
    
//     for(int i = 0; i < _num_fields; i++){
//         if(record[i][0] == '\0'){
//             break;
//         }
//         _indices[i].insert(record[i], _last_record);
//         //cout << _indices[i];
//     }
//     //cout << _indices[1] << endl;
//     // cout << "\n\n\n\n";
//     //     cout << _indices[2];
//     // cout << "\n\n\n\n";
//     //record.push_back("\0");               
//     vector<string> copy = record;
//     copy.push_back("\0");                        //potential issue?
//     FileRecord r(copy);
//     fstream f;
//     string binFileName = _table_name + ".bin";
//     open_fileRW(f, binFileName.c_str());
//     r.write(f);
//     _last_record++;
//     f.close();
// }

// Table Table::select(vector<string> desiredFields, string p, string desiredOperator, string q){
//     //cout << "DESIREDFIELDS: " << desiredFields << endl;

//     _records_PQ.clear();
//     string desiredTableName;
//     if(desiredOperator == ">"){
//         desiredTableName = p + "GT" + q;
//     } else if(desiredOperator == ">="){
//         desiredTableName = p + "GTE" + q;
//     } else if(desiredOperator == "<"){
//         desiredTableName = p + "LT" + q;
//     } else if(desiredOperator == "<="){
//         desiredTableName = p + "LTE" + q;
//     } else{
//         desiredTableName = p + desiredOperator + q;
//     }
//     Table return_me(desiredTableName, desiredFields);
//     //cout << "RETURN ME FIELD NAMES: " << return_me._field_names << endl;
//     int i = 0;
//     while(i < _field_names.size() && p != _field_names[i]){
//         i++;
//     }
//     //cout << "i: " << i << " num fields: " << _num_fields << "_fields: " << _field_names << endl;
//     assert(i != _field_names.size() && "field was not found.");

//     vector<long> recordsPQ;

//     if(desiredOperator == "="){
//         recordsPQ = _indices[i].at(q);
//     } else if(desiredOperator == ">"){
//         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
//         while(start != _indices[i].end()){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(desiredOperator == ">="){
//         recordsPQ = _indices[i].at(q);
//         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
//         while(start != _indices[i].end()){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(desiredOperator == "<"){
//         MMap<string, long>::Iterator start = _indices[i].begin();
//         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
//         while(start != end){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(desiredOperator == "<="){
//         MMap<string, long>::Iterator start = _indices[i].begin();
//         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
//         end++;
//         while(start != end){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     }

//     _records_PQ = recordsPQ;
    
//     //cout << "recordspq: " << _records_PQ << endl;

//     fstream f;
//     string tempFileName = _table_name + ".bin";
//     open_fileRW(f, tempFileName.c_str());
//     FileRecord r;
//     for(int i = 0; i < recordsPQ.size(); i++){
//         long bytes = r.read(f, recordsPQ[i]);
//         if(bytes > 0){
//             vector<string> recordToVector;
            
//             for(int i = 0; i < desiredFields.size(); i++){
//                 int j = 0;
//                 while(desiredFields[i] != _field_names[j]){
//                     j++;
//                 }
//                 recordToVector.push_back(r._record[j]);
//                 //recordToVector.push_back(r._record[i]);

//             }
//             return_me.insert_into(recordToVector);
//         }
//     }

//     f.close();
//     //cout << return_me._indices[0];
//     //swap(return_me._indices[0], return_me._indices[2]);
//     //cout << return_me._indices[0];                          //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
//     //swap(return_me._indices[0], return_me._indices[1]);
//     return return_me;
// }

// Table Table::select(vector<string> desiredFields, Queue<Token *> postfix){
//     // cout << "\nFIELD NAMES: " << _field_names << endl;
//     // cout << "DESIRED FIELDS: " << desiredFields << endl;
//     // cout << "NUM OF FIELDS: " << _num_fields << endl << endl;
//     // string newFileName = "tempPostFix";
//     // int c = 1;
//     // while(file_exists(newFileName.c_str())){
//     //     newFileName = "tempPostFix(" + to_string(c) + ")";
//     //     c++;
//     // }
//     string baseName = "tempPostFix";
//     string newFileName = baseName;

//     //newFileName = _table_name;

//     int c = 1;
//     while (file_exists((newFileName + ".bin").c_str())) {
//         newFileName = baseName + "(" + to_string(c) + ")";
//         c++;
//     }
//     //cout << "NEWFILENAME: " << newFileName << endl;
//     Table return_me(newFileName, desiredFields);
    
    
//     //return return_me;   //testing stub


//     _records_PQ.clear();
//     Stack<Token*> token_stack;
//     token_stack = Stack<Token *>();
//     Stack<vector<long>> record_stack;
//     record_stack = Stack<vector<long>>();

//     vector<long> inds;
//     int j = 0;
//     for(int i = 0; i < _num_fields; i++){
//         if(j < desiredFields.size() && _field_names[i] == desiredFields[j]){
//             inds.push_back(i);
//             j++;
//         }
//     }

//     //cout << "INDS: " << inds << endl;
//     //int iterations = 0;
//     while(!postfix.empty()){
//         //cout << "ITERATION: " << iterations << ", ";
//         Token* popped = postfix.pop();
//         //cout << "popped: "; popped->print(); cout << endl;
//         if(popped->getType() == 0){
//             //cout << "type = 0 at popped = "; popped->print(); cout << endl;
//             token_stack.push(popped);
//         } else if(popped->getType() == 1){
//             //cout << "type = 1 at popped = "; popped->print(); cout << endl;
//             Relational* op = static_cast<Relational*>(popped);
//             string desiredOperator = op->getValue();
//             TokenStr* RHS = static_cast<TokenStr*>(token_stack.pop());
//             TokenStr* LHS = static_cast<TokenStr*>(token_stack.pop());
//             string p = LHS->getValue();
//             string q = RHS->getValue();
//             record_stack.push(table_to_vector(p, desiredOperator, q));
//         } else if(popped->getType() == 2){
//             Logical* log = static_cast<Logical*>(popped);
//             string desiredLogical = log->getValue();
//             vector<long> RHS = record_stack.pop();
//             vector<long> LHS = record_stack.pop();
//             record_stack.push(applyLogical(LHS, RHS, desiredLogical));
//         }
//         //iterations++;
//     }

//     vector<long> endRecords = record_stack.pop();
//     _records_PQ = endRecords;

//     fstream f;
//     string tempFileName = _table_name + ".bin";
//     //cout << "tempFileName: " << tempFileName << endl;
//     open_fileRW(f, tempFileName.c_str());
//     FileRecord r;
//     //cout << "END RECORDS: " << endRecords << endl;
//     //cout << "INDS: " << inds << endl;
//     for(int i = 0; i < endRecords.size(); i++){
//         //cout << "hello";
//         long bytes = r.read(f, endRecords[i]);
//         cout << "bytes: " << bytes << endl;
//         if(bytes > 0){
//             //cout << "POTENTIAL RECORD: " << r << endl;
//             vector<string> recordToVector;
//             // for(int i = 0; i < inds.size(); i++){
//             //     int j = 0;
//             //     while(desiredFields[i] != _field_names[j]){                  //this might be an issue?
//             //         j++;
//             //     }
//             //     //cout << "r.record[j]: " << r._record[j] << endl;                      //CHANGING FOR LOOP TO INCLUDE ALL FIELDS
//             //     recordToVector.push_back(r._record[j]);
//             //     // recordToVector.push_back(r._record[i]);
//             // }

//             for(int i = 0; i < _field_names.size(); i++){
//                 recordToVector.push_back(r._record[i]);                     //ALL FIELDS
//             }

//             cout << "recordToVector: " << recordToVector << endl;

//             //cout << "before recordToVector: " << recordToVector << endl;
//             //recordToVector.push_back("\0");
//             //cout << "after recordToVector: " << recordToVector << endl;
//             return_me.insert_into(recordToVector);
//         }
//     }

//     f.close(); //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
//     return return_me;

    
// }

// Table Table::select(vector<string> desiredFields, vector<string> infix){
//     Queue<Token *> postFix;
//     postFix = Queue<Token *>();
//     Stack<Token *> converter;
//     converter = Stack<Token *>();

//     // postFix.push(new TokenStr("age"));
//     // postFix.push(new TokenStr("20"));
//     // postFix.push(new Relational("<"));
//     // postFix.push(new TokenStr("age"));
//     // postFix.push(new TokenStr("17"));
//     // postFix.push(new Relational(">"));
//     // postFix.push(new Logical("and"));
//     // postFix.push(new TokenStr("lname"));
//     // postFix.push(new TokenStr("Gomez"));
//     // postFix.push(new Relational("="));
//     // postFix.push(new Logical("or"));

//     //return select(desiredFields, postFix);


//     int parenTracker = 0;

//     for(int i = 0; i < infix.size(); i++){
//         string token = infix[i];

//         if(token == "("){
//             parenTracker++;
//             converter.push(new TokenStr(token));
//         }
//         else if(token == ")"){
//             parenTracker--;
//             while(!converter.empty() && static_cast<TokenStr*>(converter.top())->getValue() != "("){
//                 postFix.push(converter.pop());
//             }
//             if(!converter.empty()){
//                 Token* top = converter.pop();
//                 if(top){
//                     delete top;
//                 } else{
//                     assert(true && "should never get here");
//                 }
//                 //delete converter.pop();  // discard "(" and free memory
//             }
//         }
//         else if(token == "and" || token == "or" || token == "=" || token == "<" || token == ">" || token == "<=" || token == ">="){
//             while(!converter.empty()){
//                 Token* top = converter.top();
//                 string topVal;

//                 int type = top->getType();
//                 if(type == 1){
//                     topVal = static_cast<Relational*>(top)->getValue();
//                 } else if(type == 2){
//                     topVal = static_cast<Logical*>(top)->getValue();
//                 } else if(type == 0){
//                     topVal = static_cast<TokenStr*>(top)->getValue();
//                 }

//                 int topPriority;
//                 int tokenPriority;
//                 topPriority = getPriority(topVal);
//                 tokenPriority = getPriority(token);


//                 if(topPriority >= tokenPriority){
//                     postFix.push(converter.pop());
//                 } else{
//                     break;
//                 }
//             }

//             if(token == "and" || token == "or"){
//                 converter.push(new Logical(token));
//             } else{
//                 converter.push(new Relational(token));
//             }
//         } else{
//             postFix.push(new TokenStr(token));
//         }
//     }

//     assert(parenTracker == 0 && "uneven parenthesis");

//     while(!converter.empty()){
//         Token* push_me = converter.pop();
//         if(!push_me){
//             cout << "nullptr in stack";
//         }
//         postFix.push(push_me);
//     }

//     // 15
//     // Token* test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     //cout << "age 30 > lname Doe = and age 18 > fname Flo = and or";
//     // test = postFix.pop();
//     // test->print(); cout << " ";
//     //cout << "\n\n\n\n";
    

//     return select(desiredFields, postFix);
// }

// int getPriority(const string& input){
//     if(input == "and" || input == "or"){ return 1;}
//     if(input == "=" || input == "<" || input == ">" || input == "<=" || input == ">="){return 2;}
//     return 0;
// }

// vector<long> Table::table_to_vector(string p, string op, string q){
//     // cout << "\n\n\nFIRED TABLE_TO_VECTOR WITH: " << endl;
//     // //cout << "DESIREDFIELDS: " << desiredFields << endl;
//     // cout << "P: " << p << endl;
//     // cout << "OP: " << op << endl;
//     // cout << "Q: " << q << endl;
//     int i = 0;
//     while(i < _field_names.size() && p != _field_names[i]){
//         i++;
//     }
//     //assert(i != _field_names.size() && "field was not found.");

//     vector<long> recordsPQ;

//     if(op == "="){
//         recordsPQ = _indices[i].at(q);
//         //cout << "\n\n\n\n       RECORDSPQ: " << recordsPQ << "\n\n\n\n";
//     } else if(op == ">"){
//         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
//         while(start != _indices[i].end()){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(op == "<"){
//         MMap<string, long>::Iterator start = _indices[i].begin();
//         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
//         while(start != end){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(op == ">="){
//         recordsPQ = _indices[i].at(q);
//         MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
//         while(start != _indices[i].end()){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     } else if(op == "<="){
//         //cout << "HELLLLLLOOOOOO" << endl;
//         MMap<string, long>::Iterator start = _indices[i].begin();
//         MMap<string, long>::Iterator end = _indices[i].lower_bound(q);
//         end++;
//         while(start != end){
//             for(int i = 0; i < (*start).value_list.size(); i++){
//                 recordsPQ.push_back((*start).value_list[i]);
//             }
//             start++;
//         }
//     }
//     //cout << "RECORDS PQ: " << recordsPQ << endl;
//     return recordsPQ;
// }

// Table Table::vectorToTable(vector<long> desiredRecords, string tName){
//     // fstream f;
//     // open_fileRW(f, _table_name.c_str() + '.bin');
//     // FileRecord r;


//     return Table();
// }

// vector<long> Table::select_recnos(){

//     return _records_PQ;
// }

// ostream &operator<<(ostream &outs, const Table &t){
//     outs << "TABLE: " << t._table_name << endl;
//     //outs << "num of fields: " << t._num_fields << endl;
//     if(t._num_fields == 0){
//         return outs;
//     } //outs << t._field_names[0];
//     for(int i = 0; i < t._num_fields; i++){
//         outs << setw(25) << t._field_names[i];
//     } 
    
//     outs << endl;
//     fstream f;
//     FileRecord r2;
//     string binName = t._table_name + ".bin";
//     open_fileRW(f, binName.c_str());
//     int i = 0;
//     long bytes = r2.read(f, i);
//     while(bytes > 0){
//       outs << r2 << endl;
//       i++;
//       bytes = r2.read(f, i);
//     }
//     f.close();
//     return outs;
// }

// vector<long> applyLogical(vector<long> LHS, vector<long> RHS, string logical){
//     vector<long> result;

//     sort(RHS.begin(), RHS.end());
//     sort(LHS.begin(), LHS.end());
//     //vector<long> result;
//     if(logical == "and"){
//         set_intersection(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
//     } else if(logical == "or"){
//         set_union(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
//     }

//     // if(logical == "or"){
//     //     for(int i = 0; i < LHS.size(); i++){
//     //         bool exists = false;
//     //         for(int j = 0; j < result.size(); j++){
//     //             if(result[j] == LHS[i]){
//     //                 exists = true;
//     //                 break;
//     //             }
//     //         }
//     //         if(!exists){
//     //             result.push_back(LHS[i]);
//     //         }
//     //     }

//     //     for(int i = 0; i < RHS.size(); i++){
//     //         bool exists = false;
//     //         for(int j = 0; j < result.size(); j++){
//     //             if(result[j] == RHS[i]){
//     //                 exists = true;
//     //                 break;
//     //             }
//     //         }
//     //         if(!exists){
//     //             result.push_back(RHS[i]);
//     //         }
//     //     }
//     // } else if(logical == "and"){
//     //     for(int i = 0; i < LHS.size(); i++){
//     //         bool in_rhs = false;
//     //         for(int j = 0; j < RHS.size(); j++){
//     //             if(LHS[i] == RHS[j]){
//     //                 in_rhs = true;
//     //                 break;
//     //             }
//     //         }

//     //         if(in_rhs){
//     //             bool already_in = false;
//     //             for(int k = 0; k < result.size(); k++){
//     //                 if(result[k] == LHS[i]){
//     //                     already_in = true;
//     //                     break;
//     //                 }
//     //             }
//     //             if(!already_in){
//     //                 result.push_back(LHS[i]);
//     //             }
//     //         }
//     //     }
//     // }

//     return result;
// }


//passes table

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
    // remove(textFileName.c_str());
    // remove(binFileName.c_str());


    ofstream f2;
    f2.open(textFileName.c_str());
    //f2 << _num_fields << endl;
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
        //cout << "did i crash?";

    fstream f;
    string binFileName = tableName + ".bin";
    string txtFileName = tableName + "_fields.txt";
    _table_name = tableName;
    _last_record = 0;
    f.open(txtFileName.c_str());
    string input;
    // if(getline(f, input)){
    //     _num_fields = stoi(input);
    // }
    // for(int i = 0; i < _num_fields; i++){
    //     getline(f, input);
    //     cout << "input: " << input << endl;
    //     _field_names.push_back(input);
    // }
    while(getline(f, input)){
        //cout << "input: " << input << endl;
        _field_names.push_back(input);
    }
    _num_fields = _field_names.size();
    //cout << "FIELD NAMES FROM STRING CONSTRUCTOR: " << _field_names << endl;
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
    //cout << _indices[1] << endl;
    for(int i = 0; i < _field_names.size(); i++){
        _indices[i].insert(record[i], _last_record);
    }
}

void Table::insert_into(const vector<string>& record){
    //cout << "start of insert_into" << endl;
    
    for(int i = 0; i < _field_names.size(); i++){
        if(record[i][0] == '\0'){
            break;
        }
        _indices[i].insert(record[i], _last_record);
        //cout << _indices[i];
    }
    //cout << _indices[1] << endl;
    // cout << "\n\n\n\n";
    //     cout << _indices[2];
    // cout << "\n\n\n\n";
    //record.push_back("\0");               
    vector<string> copy = record;
    copy.push_back("\0");                        //potential issue?
    FileRecord r(copy);
    fstream f;
    string binFileName = _table_name + ".bin";
    open_fileRW(f, binFileName.c_str());
    r.write(f);
    _last_record++;
    f.close();
}

Table Table::select(vector<string> desiredFields, string p, string desiredOperator, string q){
    //cout << "DESIREDFIELDS: " << desiredFields << endl;

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
    //cout << "RETURN ME FIELD NAMES: " << return_me._field_names << endl;
    int i = 0;
    while(i < _field_names.size() && p != _field_names[i]){
        i++;
    }
    //cout << "i: " << i << " num fields: " << _num_fields << "_fields: " << _field_names << endl;
    //assert(i != _field_names.size() && "field was not found.");

    vector<long> recordsPQ;

    if(desiredOperator == "="){
        recordsPQ = _indices[i].at(q);
    } else if(desiredOperator == ">"){
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(desiredOperator == ">="){
        recordsPQ = _indices[i].at(q);
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
    
    //cout << "recordspq: " << _records_PQ << endl;

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
                //recordToVector.push_back(r._record[i]);

            }
            return_me.insert_into(recordToVector);
        }
    }

    f.close();
    //cout << return_me._indices[0];
    //swap(return_me._indices[0], return_me._indices[2]);
    //cout << return_me._indices[0];                          //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
    //swap(return_me._indices[0], return_me._indices[1]);
    return return_me;
}

Table Table::select(vector<string> desiredFields, Queue<TokenTable *> postfix){
    // cout << "\nFIELD NAMES: " << _field_names << endl;
    // cout << "DESIRED FIELDS: " << desiredFields << endl;
    // cout << "NUM OF FIELDS: " << _num_fields << endl << endl;
    //string newFileName = "tempPostFix";
    // int c = 1;
    // while(file_exists(newFileName.c_str())){
    //     newFileName = "tempPostFix(" + to_string(c) + ")";
    //     c++;
    // }
    string baseName = "tempPostFix";
    string newFileName = baseName;

    //newFileName = _table_name;

    int c = 1;
    while (file_exists((newFileName + ".bin").c_str())) {
        newFileName = baseName + "(" + to_string(c) + ")";
        c++;
    }
    //cout << "NEWFILENAME: " << newFileName << endl;
    Table return_me(newFileName, desiredFields);
    
    
    //return return_me;   //testing stub


    _records_PQ.clear();
    Stack<TokenTable*> token_stack;
    token_stack = Stack<TokenTable *>();
    Stack<vector<long>> record_stack;
    record_stack = Stack<vector<long>>();

    // vector<long> inds;
    // int j = 0;
    // for(int i = 0; i < _num_fields; i++){
    //     if(j < desiredFields.size() && _field_names[i] == desiredFields[j]){
    //         inds.push_back(i);
    //         j++;
    //     }
    // }

    //cout << "INDS: " << inds << endl;
    //int iterations = 0;
    // cout << "BALLALALALLAALLSSSSS";
    while(!postfix.empty()){
        //cout << "ITERATION: " << iterations << ", ";
        TokenTable* popped = postfix.pop();
        //cout << "popped: "; popped->print(); cout << endl;
        if(popped->getType() == 0){
            //cout << "type = 0 at popped = "; popped->print(); cout << endl;
            token_stack.push(popped);
        } else if(popped->getType() == 1){
            //cout << "type = 1 at popped = "; popped->print(); cout << endl;
            Relational* op = static_cast<Relational*>(popped);
            string desiredOperator = op->getValue();
            TokenStr* RHS = static_cast<TokenStr*>(token_stack.pop());
            TokenStr* LHS = static_cast<TokenStr*>(token_stack.pop());
            string p = LHS->getValue();
            string q = RHS->getValue();
            //cout << "\np: " << p << endl << "q: " << q << endl;
            vector<long> potentialInsert = table_to_vector(p, desiredOperator, q);
            if(!potentialInsert.empty()){
                //cout << "HELLO IM GAY\n\n\n";
                record_stack.push(potentialInsert);
            } else{
                cout << "after wrong insert!";
            }
            
        } else if(popped->getType() == 2){
            //cout << "\ngot here\n";
            Logical* log = static_cast<Logical*>(popped);
            string desiredLogical = log->getValue();
            vector<long> RHS = record_stack.pop();
            vector<long> LHS = record_stack.pop();
            //cout << "\nafter that\n";
            record_stack.push(applyLogical(LHS, RHS, desiredLogical));
        }
        //iterations++;
    }

    vector<long> endRecords = record_stack.pop();
    _records_PQ = endRecords;

    fstream f;
    string tempFileName = _table_name + ".bin";
    open_fileRW(f, tempFileName.c_str());
    FileRecord r;
    //cout << "END RECORDS: " << endRecords << endl;
    //cout << "INDS: " << inds << endl;
    for(int i = 0; i < endRecords.size(); i++){
        long bytes = r.read(f, endRecords[i]);

        if(bytes > 0){
            //cout << "POTENTIAL RECORD: " << r << endl;
            vector<string> recordToVector;
            // for(int i = 0; i < inds.size(); i++){
            //     int j = 0;
            //     while(desiredFields[i] != _field_names[j]){                  //this might be an issue?
            //         j++;
            //     }
            //     //cout << "r.record[j]: " << r._record[j] << endl;                      //CHANGING FOR LOOP TO INCLUDE ALL FIELDS
            //     recordToVector.push_back(r._record[j]);
            //     // recordToVector.push_back(r._record[i]);
            // }

            for(int i = 0; i < _field_names.size(); i++){
                recordToVector.push_back(r._record[i]);                     //ALL FIELDS
            }

            //cout << "before recordToVector: " << recordToVector << endl;
            //recordToVector.push_back("\0");
            //cout << "after recordToVector: " << recordToVector << endl;
            return_me.insert_into(recordToVector);
        }
    }

    //cout << "endrecords: " << endRecords << endl;
    return_me._records_PQ = endRecords;

    f.close(); //MIGHT NEED TO SWAP THE INDICE MAPS IN RESPONSE TO A DIFFERENT ORDER IN RESPONSE TO THE DESIRED FIELDS
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
                //delete converter.pop();  // discard "(" and free memory
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

    //assert(parenTracker == 0 && "uneven parenthesis");

    while(!converter.empty()){
        TokenTable* push_me = converter.pop();
        if(!push_me){
            cout << "nullptr in stack";
        }
        postFix.push(push_me);
    }

    Queue<TokenTable*> test = postFix;
    cout << "postfix: ";
    while(!test.empty()){
        TokenTable* p = test.pop();
        p->print(); cout << " ";
    } cout << endl;
    

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
    // cout << "\n\n\nFIRED TABLE_TO_VECTOR WITH: " << endl;
    // //cout << "DESIREDFIELDS: " << desiredFields << endl;
    // cout << "P: " << p << endl;
    // cout << "OP: " << op << endl;
    // cout << "Q: " << q << endl;
    int i = 0;
    while(i < _field_names.size() && p != _field_names[i]){
        i++;
    }
    //assert(i != _field_names.size() && "field was not found.");

    vector<long> recordsPQ;
    if(i == _field_names.size()){
        return recordsPQ;
    }

    if(op == "="){
        if(_indices[i].contains(q)){
            recordsPQ = _indices[i].at(q);
        }   //cout << "im so confused";
        return recordsPQ;
        //cout << "this is probably the problem!";
        //cout << "\n\n\n\n       RECORDSPQ: " << recordsPQ << "\n\n\n\n";
    } else if(op == ">"){
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
        if(_indices[i].contains(q)){
            recordsPQ = _indices[i].at(q);
        }
        //recordsPQ = _indices[i].at(q);
        MMap<string,long>::Iterator start = _indices[i].upper_bound(q);
        while(start != _indices[i].end()){
            for(int i = 0; i < (*start).value_list.size(); i++){
                recordsPQ.push_back((*start).value_list[i]);
            }
            start++;
        }
    } else if(op == "<="){
        //cout << "HELLLLLLOOOOOO" << endl;
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
    //cout << "RECORDS PQ: " << recordsPQ << endl;
    return recordsPQ;
}

Table Table::vectorToTable(vector<long> desiredRecords, string tName){
    // fstream f;
    // open_fileRW(f, _table_name.c_str() + '.bin');
    // FileRecord r;


    return Table();
}

vector<long> Table::select_recnos(){
    return _records_PQ;
}

ostream &operator<<(ostream &outs, const Table &t){
    outs << "TABLE: " << t._table_name << endl;
    //outs << "num of fields: " << t._num_fields << endl;
    if(t._field_names.size() == 0){
        return outs;
    } //outs << t._field_names[0];
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
    //vector<long> result;
    if(logical == "and"){
        set_intersection(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
    } else if(logical == "or"){
        set_union(RHS.begin(),RHS.end(),LHS.begin(),LHS.end(),back_inserter(result));
    }

    // if(logical == "or"){
    //     for(int i = 0; i < LHS.size(); i++){
    //         bool exists = false;
    //         for(int j = 0; j < result.size(); j++){
    //             if(result[j] == LHS[i]){
    //                 exists = true;
    //                 break;
    //             }
    //         }
    //         if(!exists){
    //             result.push_back(LHS[i]);
    //         }
    //     }

    //     for(int i = 0; i < RHS.size(); i++){
    //         bool exists = false;
    //         for(int j = 0; j < result.size(); j++){
    //             if(result[j] == RHS[i]){
    //                 exists = true;
    //                 break;
    //             }
    //         }
    //         if(!exists){
    //             result.push_back(RHS[i]);
    //         }
    //     }
    // } else if(logical == "and"){
    //     for(int i = 0; i < LHS.size(); i++){
    //         bool in_rhs = false;
    //         for(int j = 0; j < RHS.size(); j++){
    //             if(LHS[i] == RHS[j]){
    //                 in_rhs = true;
    //                 break;
    //             }
    //         }

    //         if(in_rhs){
    //             bool already_in = false;
    //             for(int k = 0; k < result.size(); k++){
    //                 if(result[k] == LHS[i]){
    //                     already_in = true;
    //                     break;
    //                 }
    //             }
    //             if(!already_in){
    //                 result.push_back(LHS[i]);
    //             }
    //         }
    //     }
    // }

    return result;
}

Table operator+(const Table& lhs, const Table& rhs){
    return Table(); //unused
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