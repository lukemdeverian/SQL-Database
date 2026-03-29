#ifndef FILE_RECORD_H
#define FILE_RECORD_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <set>
#include <vector>

using namespace std;
static const int MAX = 100;

class FileRecord
{
public:
    char _record[MAX + 1][MAX+1];
    // when you construct a Record, it's either empty or it
    //   contains a word
    FileRecord()
    {
        //_record[0][0] = '\0';
        for(int i = 0; i < MAX+1; i++){
            _record[i][0] = '\0';
        }
        recno = -1;
    }

    FileRecord(char str[][MAX+1])
    {
        for(int i = 0; i < MAX + 1; i++){
            strncpy(_record[i], str[i], MAX);
        }
    }
    FileRecord(vector<string> v){
        for(int i = 0; i < v.size(); i++){
            strncpy(_record[i], v[i].c_str(), MAX);
        }
    }

    FileRecord(string s){
        for(int i = 0; i < MAX+1; i++){
            strncpy(_record[i], s.c_str(), MAX);
        }
    }

    long write(fstream &outs);           // returns the record number
    long read(fstream &ins, long recno); // returns the number of bytes
                                         //       read = MAX, or zero if
                                         //       read passed the end of file

    friend ostream &operator<<(ostream &outs, const FileRecord &r);
    
private:
    
    int recno;
};

void open_fileW(fstream &f, const char filename[]);
void open_fileRW(fstream &f, const char filename[]);
bool file_exists(const char filename[]);

#endif