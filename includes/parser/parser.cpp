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
    string condition = "";
    bool where_present = false;
    int state = 0;

    while(!tokenized.empty()){
        string token = tokenized.pop();
        //cout << "token: " << token << endl;
        int col;
        if(keywords.contains(token)){
            col = keywords[token];
        } else{
            col = keywords["symbol"];
        }
        //cout << "\nthe state: " << state << "\nthe col: " << col << endl; 

        int nextState = _ptable[state][col];
        //assert(nextState != -1 && "Invalid SQL statement.\n" && "state: " && state && "\ncol: " col && "\n");
        if (nextState == -1) {
            cout << "Invalid SQL statement.\n"
                << "state: " << state << "\n"
                << "col: " << col << "\n";
            assert(false);
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
        } 
        // else if(state == 35 && col == keywords["symbol"]){
        //     if(!condition.empty()) condition += " ";
        //     condition += token;
        // } else if(state == 36 && col == keywords["="]){
        //     condition += " =";
        // } else if(state == 37 && col == keywords["symbol"]){
        //     condition += " " + token;
        // } else if(state == 38 && col == keywords["and"]){
        //     condition += " and";
        // }


        // else if(state >= 35){
        //     if(!condition.empty()){
        //         condition += " ";
        //     }
        //     condition += token;
        // }

        else if(state >= 35){
            _ptree.insert("condition", token);
        }

        // cout << "condition: " << condition << endl;
        state = nextState;
        
    }

    // if(where_present || !condition.empty()){
    //     _ptree.insert("where", "yes");
    //     _ptree.insert("condition", condition);
    // }

    if(where_present){
        _ptree.insert("where", "yes");
        //_ptree.insert("condition", condition);
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
    _ptable[11][keywords["symbol"]] = 12; // table name
    _ptable[12][keywords["fields"]] = 13;
    _ptable[13][keywords["symbol"]] = 14;
    _ptable[14][keywords[","]] = 13;
    _ptable[14][0] = 1;
    _ptable[13][0] = 1; 

    //COMMAND = INSERT
    _ptable[0][keywords["insert"]] = 20;
    _ptable[20][keywords["into"]] = 21;
    _ptable[21][keywords["symbol"]] = 22;     // table name
    _ptable[22][keywords["values"]] = 23;
    _ptable[23][keywords["symbol"]] = 24;
    _ptable[24][keywords[","]] = 23;          // more values
    _ptable[24][0] = 1;                        // accept
    _ptable[23][0] = 1;                        // accept after single value


    //COMMAND = SELECT
    _ptable[0][keywords["select"]] = 30;
    _ptable[30][keywords["*"]] = 31;
    _ptable[30][keywords["symbol"]] = 32;
    _ptable[32][keywords[","]] = 30;        // select multiple fields
    _ptable[31][keywords["from"]] = 33;
    _ptable[32][keywords["from"]] = 33;
    _ptable[33][keywords["symbol"]] = 34;   // table name
    _ptable[34][keywords["where"]] = 35;
    _ptable[35][keywords["symbol"]] = 36;
    _ptable[35][keywords["symbol"]] = 36;
    _ptable[35][keywords["("]] = 35;
    _ptable[35][keywords[")"]] = 35;
    _ptable[35][keywords[">="]] = 35;
    _ptable[35][keywords["="]] = 35;
    _ptable[35][keywords["and"]] = 35;
    _ptable[35][keywords["or"]] = 35;
    _ptable[36][keywords["="]] = 37;
    _ptable[36][keywords["="]] = 35;
    _ptable[36][keywords[">="]] = 35;
    _ptable[36][keywords["and"]] = 35;
    _ptable[36][keywords["or"]] = 35;
    _ptable[36][keywords["("]] = 35;
    _ptable[36][keywords[")"]] = 35;
    _ptable[36][keywords["symbol"]] = 35;
    _ptable[37][keywords["symbol"]] = 38;
    _ptable[38][keywords["and"]] = 35; 

    _ptable[34][0] = 1;
    _ptable[38][0] = 1;

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
}

Queue<string> Parser::setInput(char s[]){
    Queue<string> return_me = Queue<string>();

    STokenizer stk(s);
    Token t;

    while(!stk.done()){
        stk >> t;
        // if(t.token_str() == "\""){
        //     cout << "HELLLLOOOOOO" << endl;
        // }
        //cout << "TOKEN: " << t << ": LENGTH: " << t.token_str().length() << endl;
        // cout << "TYPE: " << t.type() << ", " << t.type_string() << endl;
        //cout << endl;
        //cout << "DEBUG RAW TOKEN: [" << t.token_str() << "]" << endl;


        if(t.token_str().length() != 0 && t.token_str()[0] == '"'){
            string bounds = t.token_str();

            int end_quote_pos = bounds.find('\"', 1);
            if (end_quote_pos != string::npos) {
                end_quote_pos++;
                string s1 = bounds.substr(0, end_quote_pos);
                string s2 = bounds.substr(end_quote_pos);
                // cout << "s1 = [" << s1 << "]\n";
                // cout << "s2 = [" << s2 << "]\n";
                return_me.push(s1.substr(1, s1.length()-2));

                // char recur[300];
                // strcpy(recur, s2);
                
                // Queue<string> extra = setInput(s2.c_str());
                if(s2.length() != 0){
                    char* recur = new char[s2.length() + 1];
                    strcpy(recur, s2.c_str());
                    Queue<string> extra = setInput(recur);
                    delete[] recur;
                    while(!extra.empty()){
                        return_me.push(extra.pop());
                    }
                }
                
            } else {
                cout << "No closing quote found.\n";
            }
        }
        
        else if(t.type_string() != "SPACE" && t.type_string() != "UNKNOWN"){
            return_me.push(t.token_str());
        }
    }

    // Queue<string> debug = return_me;
    // while(!debug.empty()){
    //     cout << debug.pop() << "_";
    // }
    return return_me;
}

void Parser::set_string(char s[]){
    tokenized = setInput(s);
    //cout << "\n\n\n";

    // Queue<string> test = tokenized;
    // while(!test.empty()){
    //     cout << test.pop() << "_";
    // } cout << endl;
}