#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

#include "../../includes/sql/sql.h"

using namespace std;

const vector<string> command_list = {
    "make table employee fields last, first, dep, salary, year",
    "insert into employee values Blow, Joe, CS, 100000, 2018",
    "insert into employee values Johnson, Jack, HR, 150000, 2014",
    "insert into employee values Johnson, Jimmy, Chemistry, 140000, 2018",
    "select * from employee",
    "update employee set salary = 99999 where last = Johnson",
    "select * from employee",
    "update employee set dep = Finance",
    "select * from employee"
};

bool sql_update_test(bool debug = false)
{
    cout << "test rebuild 1 \n";
    
    SQL sql;
    Table t;

    for (int i = 0; i < command_list.size(); i++)
    {
        cout << "\n>" << command_list[i] << endl;
        t = sql.command(command_list[i]);
        if (debug) {
            cout << t << endl;
        }
        cout << "records selected: " << sql.select_recnos() << endl;
    }

    cout << "\n----- UPDATE TEST COMPLETE --------" << endl;
    return true;
}

bool debug = false;

TEST(SQL_UPDATE, SQLUpdate) {
    bool success = sql_update_test(debug);
    EXPECT_EQ(success, true);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        debug = !strcmp(argv[1], "debug");
    }

    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running update_test.cpp---------\n\n" << std::endl;
    return RUN_ALL_TESTS();
}
