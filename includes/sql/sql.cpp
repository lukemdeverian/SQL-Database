#include "sql.h"

SQL::SQL(){

}

Table SQL::command(string theCommand){
    _ptree.clear();
    //Parser p(theCommand);
    char commandChar[300];
    strcpy(commandChar, theCommand.c_str());
    Parser p(commandChar);
    _ptree = p.parse_tree();

    // _ptree.print_lookup();
    // cout << endl;

    if(_ptree.empty()){
        return Table();
    }

    _records_selected.clear();

    string cmd = _ptree["command"][0];
    //cout << "cmd: " << cmd << endl;
    if(cmd == "make" || cmd == "create"){
        string tableName = _ptree["table_name"][0];
        vector<string> fields = _ptree["fields"];
        _tables[tableName] = Table(tableName, fields);
        return _tables[tableName];
    }
    else if(cmd == "insert"){
        string tableName = _ptree["table_name"][0];
        // vector<string> input = _ptree["values"];
        // _tables[tableName].insert_into(input);
        // return _tables[tableName];
        Table return_me(tableName);
        return_me.insert_into(_ptree["values"]);
        return return_me;
    }
    else if(cmd == "select"){
        string tableName = _ptree["table_name"][0];
        vector<string> fields = _ptree["fields"];
        // if(_ptree.contains("condition")){
        //     vector<string> condition = _ptree["condition"];
        //     cout << "condition: " << condition << endl;
        //     Table return_me = _tables[tableName].select(fields, condition);
        //     //cout << "DID I CRASH?";
        //     _records_selected = return_me.select_recnos();
        //     return return_me;
        // } else{
        //     Table return_me = _tables[tableName].selectAll(fields);
        //     _records_selected = return_me.select_recnos();
        //     return return_me;
        // }

        Table return_me(tableName);

        if(_ptree["fields"][0] == "*"){
            fields = return_me.getFields();
        } 

        if(_ptree.contains("where")){
            return_me.select(fields, _ptree["condition"]);
        } else{
            return_me.selectAll(fields);
        }
        _records_selected = return_me.select_recnos();
        return return_me;
        
    }
    //assert(false && "invalid SQL command");
    return Table();
}

vector<long> SQL::select_recnos(){
    return _records_selected;
}

