#include "stokenize.h"

STokenizer::STokenizer(){
    strcpy(buffer, "\0");
    _done = false;
    pos = 0;
    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COLUMNS; j++){
            table[i][j] = -1;
        }
    }
    for(int i = 0; i < 10; i++){
        table[0][DIGITS[i]] = 1;
        
        table[1][DIGITS[i]] = 1;

        table[2][DIGITS[i]] = 3;
        table[3][DIGITS[i]] = 3;
    }
    table[1][0] = 1;
    table[1]['.'] = 2;
    table[2][0] = 0;
    table[3][0] = 1;

    for(int i = 0; i < 52; i++){
        table[0][ALFA[i]] = 4;
        table[4][ALFA[i]] = 4;
    }
    table[4][0] = 1;

    for(int i = 0; i < 4; i++){
        table[0][SPACES[i]] = 5;
        table[5][SPACES[i]] = 9;
    }
    table[9][0] = 0;
    table[5][0] = 1;
    for(int i = 0; i < 9; i++){
        table[0][PUNC[i]] = 6;
        table[6][PUNC[i]] = 7;
    }
    table[6][0] = 1;
    table[7][0] = 0;
    for(int i = 0; i < 11; i++){
        table[0][OPERATORS[i]] = 8;
        table[8][OPERATORS[i]] = 10;
    }
    table[8][0] = 1;
    table[10][0] = 0;

    table[0]['"'] = 11;

    for (int i = 32; i < 127; i++) {
        if (i != '"') {
            table[11][i] = 11;
        }
    }

    table[11]['"'] = 12;

    table[12][0] = 1;
}

STokenizer::STokenizer(char input[]){
    strcpy(buffer, input);
    pos = 0;
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 127; j++){
            table[i][j] = -1;
        }
    }
    for(int i = 0; i < 10; i++){
        table[0][DIGITS[i]] = 1;
        
        table[1][DIGITS[i]] = 1;

        table[2][DIGITS[i]] = 3;
        table[3][DIGITS[i]] = 3;
    }
    table[1][0] = 1;
    table[1]['.'] = 2;
    table[3][0] = 1;

    for(int i = 0; i < 52; i++){
        table[0][ALFA[i]] = 4;
        table[4][ALFA[i]] = 4;
    }
    table[4][0] = 1;

    for(int i = 0; i < 4; i++){
        table[0][SPACES[i]] = 5;
        table[5][SPACES[i]] = 5;
    }
    table[5][0] = 1;

    for(int i = 0; i < 9; i++){
        table[0][PUNC[i]] = 6;
        table[6][PUNC[i]] = 6;
    }
    table[6][0] = 1;
    for(int i = 0; i < 12; i++){
        table[0][OPERATORS[i]] = 8;
    }
    table[8][OPERATORS[2]] = 8;
    table[8][0] = 1;

     table[0]['"'] = 11;

    for (int i = 0; i < 127; i++) {
        if (i != '"') {
            table[11][i] = 11;
        }
    }

    table[11]['"'] = 12;

    table[12][0] = 1;
}

bool STokenizer::get_token(Token &token){
    int state = 0;
    bool achievedSuccess = false;
    token.reset_token();
    int lastSuccess = -1;
    int colZeroTracker = -1;
    int posOfAttemptedToken = pos;
    int lastPos = -1;
    int lastState;

    while(buffer[pos] != '\0'){
        if(table[state][buffer[pos]] != -1){
            state = table[state][buffer[pos]];
            colZeroTracker = table[state][0];
            if(table[state][0] == 1){
                lastPos = pos;
                achievedSuccess = true;
                lastSuccess = state;
            }
            pos++;
        }
        else{
            if(achievedSuccess){
                string s = string(buffer+posOfAttemptedToken, lastPos-posOfAttemptedToken+1);
                token.append(s);
                token.setType(lastSuccess);
                pos = lastPos+1;
            } else{
                pos++;
            } return achievedSuccess;
        }
    }

    if(lastPos != -1){
        token.append(string(buffer + posOfAttemptedToken, lastPos-posOfAttemptedToken+1));
        state = table[0][buffer[lastPos]];
        token.setType(lastSuccess);
        pos = lastPos+1;
    } else{
        _done = true;
    } return achievedSuccess;
}

bool STokenizer::done()
{
    if(buffer[pos] == '\0'){
        return true;
    }
    return false;
}

bool STokenizer::more()
{
    return (!done());
}

int STokenizer::get_postion()
{
    return pos;
}

void STokenizer::setBuffer(char buff[])
{
    strcpy(buffer, buff);
    pos = 0;
    _done = false;
}

void STokenizer::set_string(char buff[])
{
    setBuffer(buff);
}

STokenizer &operator>>(STokenizer &STK, Token &token)
{
    if(STK.get_token(token)){
    
    }
    return STK;
}
