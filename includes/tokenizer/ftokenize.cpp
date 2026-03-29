#include "ftokenize.h"

FTokenizer::FTokenizer(char *fname)
{
    _f.open(fname);
    _pos = 0;
    _blockPos = 0;
    _more = true;
    constru = 0;
}

Token FTokenizer::next_token()
{
    Token t;
    if(!_stk.done()){
        _stk>>t;
        return t;
    }
    if(get_new_block()){
        _stk >> t;
        return t;
    } return t;

}

bool FTokenizer::more()
{
    return _more;
}

int FTokenizer::pos()
{
    return _pos;
}

int FTokenizer::block_pos()
{
    return _blockPos;
}

bool FTokenizer::get_new_block()
{
    char buffer[MAX_BUFFER]; 
    _f.read(buffer, MAX_BUFFER-1);
    buffer[_f.gcount()] = '\0';
    _stk.setBuffer(buffer);
    if(_f.gcount() == 0){
        _more = false;
        return _more;
    }
    return _more;
}

FTokenizer &operator>>(FTokenizer &f, Token &t)
{
    t = f.next_token();
    return f;
}
