#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include "includes/sql/sql.h"
#include "includes/html_logger.h"

using namespace std;
using namespace std::chrono;

// Helper: pull field names and all rows out of a Table for logging
void extract_table(Table& t, vector<string>& fields, vector<vector<string>>& rows) {
    fields = t.getFields();
    // Table's operator<< reads the .bin file — we replicate that logic here
    // by reading records via the select_recnos pattern.
    // Simplest approach: re-open the bin file directly.
    // Since Table doesn't expose a row-iterator, we use a fresh Table load.
    // (This is a known limitation we can improve later.)
    rows.clear();
    // We'll populate rows from the table's operator<< output by string parsing —
    // but that's fragile. Instead, expose a method or just leave rows empty
    // and rely on the HTML showing "see query output". 
    // For now, this is a placeholder — see note below.
}

// ---- Demo queries ----
const vector<string> demo_commands = {
    "make table employee fields last, first, dep, salary, year",
    "insert into employee values Blow, Joe, CS, 100000, 2018",
    "insert into employee values Blow, JoAnn, Physics, 200000, 2016",
    "insert into employee values Johnson, Jack, HR, 150000, 2014",
    "insert into employee values Johnson, Jimmy, Chemistry, 140000, 2018",
    "select * from employee",
    "select * from employee where last = Johnson",
    "select * from employee where last = Blow",
    "update employee set salary = 99999 where last = Johnson",
    "select * from employee",
    "make table student fields fname, lname, major, age",
    "insert into student values Flo, Yao, Art, 20",
    "insert into student values Bo, Yang, CS, 28",
    "insert into student values Billy, Jackson, Math, 27",
    "insert into student values Mary, Davis, Math, 30",
    "select * from student",
    "select * from student where (major=CS or major=Art)",
    "select * from student where lname>J and (major=CS or major=Art)",
};

int main() {
    SQL sql;
    HTMLLogger logger;

    cout << "Running SQL Engine demo..." << endl;

    for (const string& cmd : demo_commands) {
        cout << "> " << cmd << endl;

        auto t0 = high_resolution_clock::now();
        Table result = sql.command(cmd);
        auto t1 = high_resolution_clock::now();
        double ms = duration<double, milli>(t1 - t0).count();

        vector<string> fields = result.getFields();
        vector<long>   recnos = sql.select_recnos();

        // Build rows by re-loading from disk using the record numbers
        vector<vector<string>> rows;
        // We read the bin file for this table using a fresh Table object
        // and the record numbers returned by select_recnos.
        // Since Table doesn't expose a get_row(recno) method yet,
        // we use a simple workaround: load a fresh Table and iterate all records,
        // keeping only those whose index is in recnos.
        // For INSERT/MAKE commands, recnos is empty so rows stays empty.
        if (!fields.empty() && !recnos.empty()) {
            string tableName = "";
            // Extract table name from command
            // "select ... from TABLE" or "update TABLE ..."
            // Simple parse: find "from" or second word for update
            istringstream iss(cmd);
            string word;
            vector<string> words;
            while (iss >> word) words.push_back(word);
            
            for (int i = 0; i < (int)words.size(); i++) {
                string w = words[i];
                for (auto& c : w) c = tolower(c);
                if ((w == "from" || w == "update") && i+1 < (int)words.size()) {
                    tableName = words[i+1];
                    break;
                }
            }

            if (!tableName.empty()) {
                // Read rows from the .bin file for each recno
                string binFile = tableName + ".bin";
                fstream f;
                f.open(binFile, ios::in | ios::binary);
                if (f.is_open()) {
                    for (long recno : recnos) {
                        FileRecord r;
                        long pos = recno * sizeof(r._record);
                        f.seekg(pos, ios_base::beg);
                        f.read(&r._record[0][0], sizeof(r._record));
                        if (f.gcount() > 0) {
                            vector<string> row;
                            for (int col = 0; col < (int)fields.size(); col++) {
                                row.push_back(string(r._record[col]));
                            }
                            rows.push_back(row);
                        }
                    }
                    f.close();
                }
            }
        }

        logger.log(cmd, fields, rows, recnos, ms);
    }

    logger.write("demo_output.html");
    cout << "\nDemo complete! Open demo_output.html in your browser." << endl;
    return 0;
}
