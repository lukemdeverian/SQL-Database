#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/table/table.h"
using namespace std;

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }

  vector<long> e;
  vector<long> p = {1,2,3,4,5};
  string s = "or";
  vector<long> r = applyLogical(e, p, s);
  cout << "r: " << r << endl;
  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

