#include "sql.h"

SQL::SQL(){

}

Table SQL::command(string theCommand){
    _ptree.clear();
    char commandChar[300];
    strcpy(commandChar, theCommand.c_str());
    Parser p(commandChar);
    _ptree = p.parse_tree();

    if(_ptree.empty()){
        return Table();
    }

    _records_selected.clear();

    string cmd = _ptree["command"][0];
    if(cmd == "make" || cmd == "create"){
        string tableName = _ptree["table_name"][0];
        vector<string> fields = _ptree["fields"];
        _tables[tableName] = Table(tableName, fields);
        return _tables[tableName];
    }
    else if(cmd == "insert"){
        string tableName = _ptree["table_name"][0];
        Table return_me(tableName);
        return_me.insert_into(_ptree["values"]);
        return return_me;
    }
    else if(cmd == "select"){
        string tableName = _ptree["table_name"][0];
        vector<string> fields = _ptree["fields"];

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
    else if(cmd == "update"){
        string tableName = _ptree["table_name"][0];
        string updateField = _ptree["update_field"][0];
        string updateValue = _ptree["update_value"][0];
        Table return_me(tableName);
        if(_ptree.contains("where")){
            return_me.update(updateField, updateValue, _ptree["condition"]);
        } else{
            return_me.updateAll(updateField, updateValue);
        }
        _records_selected = return_me.select_recnos();
        return return_me;
    }
    return Table();
}

vector<long> SQL::select_recnos(){
    return _records_selected;
}

