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
    // if(constru == 0){
    //     //cout << "balls";
    //     get_new_block();
    //     constru = 1;
    // }
    Token t;
    if(!_stk.done()){
        //cout << "balls\n";

        //cout << "balls12\n";
        _stk>>t;
        return t;
    }
    if(get_new_block()){
        //cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
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
    //cout << "getting new block\n";
    char buffer[MAX_BUFFER]; 
    _f.read(buffer, MAX_BUFFER-1);
    //cout << buffer << endl;
    //cout << "\ngcount: " << _f.gcount() << endl;
    buffer[_f.gcount()] = '\0';
    //_stk = STokenizer(buffer);
    _stk.setBuffer(buffer);
    if(_f.gcount() == 0){
        //cout << "done\n";
        _more = false;
        return _more;
    }
    return _more;
}

FTokenizer &operator>>(FTokenizer &f, Token &t)
{
    t = f.next_token();
    return f;
    // TODO: insert return statement here
}
