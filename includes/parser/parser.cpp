#include "parser.h"

Parser::Parser(){
    makeKeywordsMap();
    make_ptable();
}

Parser::Parser(char s[]){
    makeKeywordsMap();
    make_ptable();
    tokenized = setInput(s);
}

mmap_ss Parser::parse_tree(){
    _ptree.clear();
    bool where_present = false;
    int state = 0;

    while(!tokenized.empty()){
        string token = tokenized.pop();
        int col;
        if(keywords.contains(token)){
            col = keywords[token];
        } else{
            col = keywords["symbol"];
        }

        int nextState = _ptable[state][col];
        if (nextState == -1) {
            cout << "Invalid SQL statement.\n"
                 << "state: " << state << "\n"
                 << "col: " << col << "\n";
            _ptree.clear();
            return _ptree;
        }

        //COMMAND = MAKE
        if(state == 0){
            _ptree.insert("command", token);
        } else if(state == 11 && col == keywords["symbol"]){
            _ptree.insert("table_name", token);
        } else if((state == 13 || state == 14) && col == keywords["symbol"]){
            _ptree.insert("fields", token);
        }

        //COMMAND = INSERT
        else if(state == 21 && col == keywords["symbol"]){
            _ptree.insert("table_name", token);
        } else if((state == 23 || state == 24) && col == keywords["symbol"]){
            _ptree.insert("values", token);
        }

        //COMMAND = SELECT
        else if(state == 30 && (col == keywords["*"] || col == keywords["symbol"])){
            _ptree.insert("fields", token);
        } else if(state == 33 && col == keywords["symbol"]){
            _ptree.insert("table_name", token);
        } else if(state == 34 && col == keywords["where"]){
            where_present = true;
        } else if(state >= 35 && state <= 38){
            _ptree.insert("condition", token);
        }

        //COMMAND = UPDATE
        else if(state == 40 && col == keywords["symbol"]){
            _ptree.insert("table_name", token);
        } else if(state == 42 && col == keywords["symbol"]){
            _ptree.insert("update_field", token);
        } else if(state == 44 && col == keywords["symbol"]){
            _ptree.insert("update_value", token);
        } else if(state == 45 && col == keywords["where"]){
            where_present = true;
        } else if(state >= 46){
            _ptree.insert("condition", token);
        }

        state = nextState;
    }

    if(where_present){
        _ptree.insert("where", "yes");
    }

    return _ptree;
}

void Parser::make_ptable(){
    for(int i = 0; i < MAX_PROWS; i++){
        for(int j = 1; j < MAX_PCOLS; j++){
            _ptable[i][j] = -1;
        } _ptable[i][0] = 0;
    }

    //COMMAND = MAKE
    _ptable[0][keywords["make"]] = 10;
    _ptable[10][keywords["table"]] = 11;
    _ptable[11][keywords["symbol"]] = 12;
    _ptable[12][keywords["fields"]] = 13;
    _ptable[13][keywords["symbol"]] = 14;
    _ptable[14][keywords[","]] = 13;
    _ptable[14][0] = 1;
    _ptable[13][0] = 1;

    //COMMAND = INSERT
    _ptable[0][keywords["insert"]] = 20;
    _ptable[20][keywords["into"]] = 21;
    _ptable[21][keywords["symbol"]] = 22;
    _ptable[22][keywords["values"]] = 23;
    _ptable[23][keywords["symbol"]] = 24;
    _ptable[24][keywords[","]] = 23;
    _ptable[24][0] = 1;
    _ptable[23][0] = 1;

    //COMMAND = SELECT
    _ptable[0][keywords["select"]] = 30;
    _ptable[30][keywords["*"]] = 31;
    _ptable[30][keywords["symbol"]] = 32;
    _ptable[32][keywords[","]] = 30;
    _ptable[31][keywords["from"]] = 33;
    _ptable[32][keywords["from"]] = 33;
    _ptable[33][keywords["symbol"]] = 34;
    _ptable[34][keywords["where"]] = 35;
    _ptable[34][0] = 1;

    // WHERE clause for SELECT
    _ptable[35][keywords["symbol"]] = 36;
    _ptable[35][keywords["("]]      = 35;
    _ptable[35][keywords[")"]]      = 35;
    _ptable[35][keywords["="]]      = 35;
    _ptable[35][keywords[">="]]     = 35;
    _ptable[35][keywords["<="]]     = 35;
    _ptable[35][keywords[">"]]      = 35;
    _ptable[35][keywords["<"]]      = 35;
    _ptable[35][keywords["and"]]    = 35;
    _ptable[35][keywords["or"]]     = 35;
    _ptable[35][0]                  = 1;

    _ptable[36][keywords["="]]      = 37;   // field = value (key transition)
    _ptable[36][keywords[">="]]     = 35;
    _ptable[36][keywords["<="]]     = 35;
    _ptable[36][keywords[">"]]      = 35;
    _ptable[36][keywords["<"]]      = 35;
    _ptable[36][keywords["and"]]    = 35;
    _ptable[36][keywords["or"]]     = 35;
    _ptable[36][keywords["("]]      = 35;
    _ptable[36][keywords[")"]]      = 35;
    _ptable[36][keywords["symbol"]] = 35;

    _ptable[37][keywords["symbol"]] = 38;
    _ptable[38][keywords["and"]]    = 35;
    _ptable[38][keywords["or"]]     = 35;
    _ptable[38][keywords[")"]]      = 35;
    _ptable[38][0]                  = 1;

    //COMMAND = UPDATE
    // UPDATE table SET field = value [, field = value] [WHERE condition]
    _ptable[0][keywords["update"]]  = 40;
    _ptable[40][keywords["symbol"]] = 41;   // table name
    _ptable[41][keywords["set"]]    = 42;
    _ptable[42][keywords["symbol"]] = 43;   // field name
    _ptable[43][keywords["="]]      = 44;
    _ptable[44][keywords["symbol"]] = 45;   // new value
    _ptable[45][keywords[","]]      = 42;   // another SET pair
    _ptable[45][keywords["where"]]  = 46;
    _ptable[45][0]                  = 1;    // accept: no WHERE

    // WHERE clause for UPDATE
    _ptable[46][keywords["symbol"]] = 47;
    _ptable[46][keywords["("]]      = 46;
    _ptable[46][keywords[")"]]      = 46;
    _ptable[46][keywords["="]]      = 46;
    _ptable[46][keywords[">="]]     = 46;
    _ptable[46][keywords["<="]]     = 46;
    _ptable[46][keywords[">"]]      = 46;
    _ptable[46][keywords["<"]]      = 46;
    _ptable[46][keywords["and"]]    = 46;
    _ptable[46][keywords["or"]]     = 46;
    _ptable[46][0]                  = 1;

    _ptable[47][keywords["="]]      = 48;
    _ptable[47][keywords[">="]]     = 46;
    _ptable[47][keywords["<="]]     = 46;
    _ptable[47][keywords[">"]]      = 46;
    _ptable[47][keywords["<"]]      = 46;
    _ptable[47][keywords["and"]]    = 46;
    _ptable[47][keywords["or"]]     = 46;
    _ptable[47][keywords["("]]      = 46;
    _ptable[47][keywords[")"]]      = 46;
    _ptable[47][keywords["symbol"]] = 46;

    _ptable[48][keywords["symbol"]] = 49;
    _ptable[49][keywords["and"]]    = 46;
    _ptable[49][keywords["or"]]     = 46;
    _ptable[49][0]                  = 1;
}

void Parser::makeKeywordsMap()
{
    keywords.insert("select", 1);
    keywords.insert("from", 2);
    keywords.insert("*", 3);
    keywords.insert("where", 4);
    keywords.insert("symbol", 5);
    keywords.insert(",", 6);

    keywords.insert("make", 7);
    keywords.insert("table", 8);
    keywords.insert("fields", 9);

    keywords.insert("insert", 10);
    keywords.insert("into", 11);
    keywords.insert("values", 12);
    keywords.insert("and", 13);
    keywords.insert("=", 14);

    keywords.insert("or", 15);
    keywords.insert("(", 16);
    keywords.insert(")", 17);
    keywords.insert(">=", 18);
    keywords.insert("<", 19);
    keywords.insert("<=", 20);
    keywords.insert(">", 21);
    keywords.insert("update", 22);
    keywords.insert("set", 23);
}

Queue<string> Parser::setInput(char s[]){
    Queue<string> return_me = Queue<string>();

    STokenizer stk(s);
    Token t;

    while(!stk.done()){
        stk >> t;

        if(t.token_str().length() != 0 && t.token_str()[0] == '"' && t.token_str() != "\"\""){
            string bounds = t.token_str();
            return_me.push(bounds.substr(1, bounds.length()-2));
        }
        else if(t.type_string() != "SPACE" && t.type_string() != "UNKNOWN"){
            return_me.push(t.token_str());
        }
    }
    return return_me;
}

void Parser::set_string(char s[]){
    tokenized = setInput(s);
}