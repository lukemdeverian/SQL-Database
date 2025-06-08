#include "file_record.h"

long FileRecord::write(fstream &outs)
{
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //outs.write(&record[0], sizeof(record));
    // outs.write(_record, sizeof(_record));
    //for(int i = 0; i < )
    outs.write(&_record[0][0], sizeof(_record));

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    //ins.read(_record, sizeof(_record));
    memset(_record, 0, sizeof(_record));
    ins.read(&_record[0][0], sizeof(_record));
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    // _record[ins.gcount()] = '\0';

    //((char*)_record)[ins.gcount()] = '\0';


    if (ins.gcount() < sizeof(_record)) {
        size_t row = ins.gcount() / (MAX+1);
        size_t col = ins.gcount() % (MAX+1);
        _record[row][col] = '\0';
    }

    return ins.gcount();

}
ostream& operator<<(ostream& outs,
                    const FileRecord& r){
    //outs<<r._record;
    // outs << "lets see:" << r._record[0] << endl;
    for(int i = 0; i < MAX + 1; i++){

        if(r._record[i][0] == '\0'){
            //cout << "HELLO";
            return outs;
        } else{
            if(static_cast<int>(r._record[i][0]) < 0 || static_cast<int>(r._record[i][0]) > 127){
                return outs;
            }
            outs << setw(25) << r._record[i];
            //cout << "ascii: " << static_cast<int>(r._record[i][0]);
        }
    } return outs;
}


//----------------------------------------------------------------------------
//                      F I L E   F U N C T I O N S 
//----------------------------------------------------------------------------



//[    |    |     |    |    |     |]
//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]){
    const bool debug = false;
    //opening a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

// ostream &operator<<(ostream &outs, const FileRecord &r){
//     outs << r.my_to_string();
//     return outs;
// }