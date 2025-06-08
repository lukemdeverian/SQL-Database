#include "token.h"

Token::Token(){
    _token = "";
    _type = 0;
}

Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}

int Token::type() const
{
    return _type;
}

string Token::type_string() const
{
    if(_type == TOKEN_NUMBER || _type == 3){
        return "NUMBER";
    } else if(_type == TOKEN_ALPHA){
        return "ALPHA";
    } else if(_type == TOKEN_END){
        return "END";
    } else if(_type == TOKEN_OPERATOR){
        return "OPERATOR";
    } else if(_type == TOKEN_PUNC){
        return "PUNCUATION";
    } else if(_type == TOKEN_SPACE){
        return "SPACE";
    } else{
        return "UNKNOWN";
    }
}

string Token::token_str() const
{
    return _token;
}

void Token::reset_token()
{
    _token = "";
}

void Token::append(char c)
{
    _token += c;
}

void Token::append(string s)
{
    _token = s;
}

void Token::setType(int type){
    _type = type;
}

void Token::removeEnd()
{
    _token.pop_back();
}

ostream &operator<<(ostream &outs, const Token &t)
{
    // TODO: insert return statement here
    outs << t.token_str();
    return outs;
}
