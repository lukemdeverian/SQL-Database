#include "stokenize.h"

STokenizer::STokenizer(){
    //cout << "calls me";
    //strcpy(buffer, " in Aracataca, Colombia in 1928, but he has lived most of his life in Mexico and Europe. He attended the University of Bogotá and later worked as staff reporter and film critic for the Colombian newspaper El Espectador. In addition to ONE HUNDRED YEARS OF SOLITUDE. he has also written two collections of short fiction, NO ONE WRITES TO THE COLONEL and LEAF STORM (both available in Bard editions).");
    //strcpy(buffer, " 2. ");
    //strcpy(buffer, "123.");
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
    //maybe make special case for ...
    for(int i = 0; i < 11; i++){
        table[0][OPERATORS[i]] = 8;
        table[8][OPERATORS[i]] = 10;
    }
    table[8][0] = 1;
    table[10][0] = 0;
    //table[0][0] = 1;

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
    //cout << buffer << endl;
    pos = 0;
    for(int i = 0; i < 10; i++){
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
    //maybe make special case for ...
    for(int i = 0; i < 12; i++){
        table[0][OPERATORS[i]] = 8;
        //table[8][OPERATORS[i]] = 8;
    }
    table[8][OPERATORS[2]] = 8;
    table[8][0] = 1;

     table[0]['"'] = 11;

    for (int i = 32; i < 127; i++) {
        if (i != '"') {
            table[11][i] = 11;
        }
    }

    table[11]['"'] = 12;

    table[12][0] = 1;
}

bool STokenizer::get_token(Token &token){
    //cout << "the buffer: " << buffer << endl;
    //cout << "balls";
    int state = 0;
    bool achievedSuccess = false;
    token.reset_token();
    int lastSuccess = -1;
    int colZeroTracker = -1;
    //int neutralCheck = -1;
    int posOfAttemptedToken = pos;
    //int lastPos = pos;
    int lastPos = -1;
    int lastState;
    //cout << pos << endl;
    // if(buffer[pos] == '\0'){
    //     return false;
    // }
    // if(buffer[pos] < 0 || buffer[pos] > 127){
    //     pos++;
    //     //lastPos = pos;
    // }

    while(buffer[pos] != '\0'){
        //cout << "test\n";
        //cout << "pos: " << pos <<endl;
        //cout << "state: " << state << endl;

        //state = table[state][buffer[pos]];
        // if(buffer[pos] < 0 || buffer[pos] > 127){
        //     //pos++;
        //     lastPos = pos;
        //     break;
        // }
        if(table[state][buffer[pos]] != -1){
            state = table[state][buffer[pos]];
            colZeroTracker = table[state][0];
            if(table[state][0] == 1){
                //cout << "eee\n";
                lastPos = pos;
                achievedSuccess = true;
                lastSuccess = state;
                // if(state == 6 || state == 5){
                //     break;
                // }
            }
            pos++;
        }
        else{
            //cout << "bbb";
            if(achievedSuccess){
                string s = string(buffer+posOfAttemptedToken, lastPos-posOfAttemptedToken+1);
                //cout << s << endl;
                token.append(s);
                token.setType(lastSuccess);
                pos = lastPos+1;
            } else{
                pos++;
            } return achievedSuccess;
        }
    }

    if(lastPos != -1){
        //cout << "balls";
        //cout << string(buffer + posOfAttemptedToken, lastPos-posOfAttemptedToken+1) << endl;
        token.append(string(buffer + posOfAttemptedToken, lastPos-posOfAttemptedToken+1));
        state = table[0][buffer[lastPos]];
        token.setType(lastSuccess);
        pos = lastPos+1;
        //cout << pos;
    } else{
        _done = true;
    } return achievedSuccess;

    // while(buffer[pos] != '\0'){
    //     //cout << "state: " << state << endl;
    //     //cout << "buffer[pos]: " << buffer[pos] << endl;
    //     //cout << table[state][0] << endl;
    //     if(buffer[pos] < 0 || buffer[pos] > 127){
    //         //pos++;
    //         lastPos = pos+1;
    //         break;
    //     }

    //     if(table[state][0] == 1){
    //         //cout << "ajdsklfljksd\n";
    //         achievedSuccess = true;
    //         lastSuccess = state;
    //         //lastPos = pos;
    //     }

    //     if(table[state][0] == 0){
    //         cout << "bad\n";
    //         pos++;
    //         break;
    //         //return false;
    //     }

    //     if(table[state][buffer[pos]] != -1 /*&& table[state][0] == 1*/){
    //         //cout << "hellllo";
    //         //token.setType(state);
    //         //token.append(buffer[pos]);
    //         state = table[state][buffer[pos]];
            
    //         lastState = state;
    //         if(table[lastState][0] == 1){
    //             lastSuccess = state;
    //             lastPos = pos;
    //         }
    //         if(state == 6){
    //             break;
    //         }
    //     } else{
    //         //pos--;
    //         break;
    //     }
    //     pos++;
    // }

    // // if(posOfAttemptedToken == lastPos && (buffer[posOfAttemptedToken] < 0 || buffer[posOfAttemptedToken] > 127)){
    // //     pos = lastPos+1;
    // //     //cout << "hellllllooooooo";
    // //     return false;
    // // }

    // if(lastSuccess != -1){
    //     //cout << "balls";
    //     token.append(string(buffer + posOfAttemptedToken, lastPos-posOfAttemptedToken+1));
    //     token.setType(lastSuccess);
    //     pos = lastPos+1;
    //     //cout << pos;
    // } else{
    //     _done = true;
    // } return achievedSuccess;

    //cout << "laststate: " << lastState << endl;
    // if(lastState == 2){
    //     lastPos--;
    // }
    //cout << "\nbeginpos" << posOfAttemptedToken<< " pos: " << pos << " lastPos: " << lastPos << endl;
    // for(int i = posOfAttemptedToken; i <= lastPos; i++){
    //     token.append(buffer[i]);
    // }
    // cout << pos << endl;
    //pos = lastPos+1;
    // cout << pos << endl;
    //cout << endl;
    //token.setType(lastState);
    //return true;
}

bool STokenizer::done()
{
    //cout << "call\n";
    //cout << buffer << endl;
    //cout << pos << endl;
    if(buffer[pos] == '\0'){
        //cout << "im done\n";
        return true;
    }
    //cout << buffer[pos] << endl;
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
    //cout << buffer << endl;
    pos = 0;
    _done = false;
}

void STokenizer::set_string(char buff[])
{
    setBuffer(buff);
}

STokenizer &operator>>(STokenizer &STK, Token &token)
{
    //cout << "balls\n";
    if(STK.get_token(token)){
         //cout << token.type_string() << " " << token << endl;
    }
    return STK;
}
