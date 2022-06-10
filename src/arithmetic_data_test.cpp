#include "BinarySerialization.h"

int main(){
  std::cout << "Check Point 0" << std::endl;
  /*0. Create test file */
  std::fstream test_file;
  test_file.open("arithmetic_data_testf.dat", std::ios::out|std::ios::binary);
  if(!test_file.is_open()) {
    std::cout << "Fail to open .dat file ";
  }
  std::cout << "Check Point 1" << std::endl;

  /*1. Create an executor object of for test */
  test_file.seekp(0, std::ios::beg);
  mkbs::BinSerExecutor test_bin_ser_executor;
  std::cout << "Check Point 2" << std::endl;

  /*2. Enqueue the arithmetic value */
  bool   t_bool   = true;
  char   t_char   = 'a';
  int    t_int    = 100;
  float  t_float  = 99.99;
  double t_double = 100.00001;

  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_bool), "Enqueue bool val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_char), "Enqueue char val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_int), "Enqueue int val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_float), "Enqueue float val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_double), "Enqueue double val fails");
  std::cout << "Check Point 3" << std::endl;

  /*3. Serialize all the data into file */
  ASSERT_EQ(true, test_bin_ser_executor.SerializeToDataFile(test_file), "Serialize fails");
  std::cout << "Check Point 4" << std::endl;
  test_file.close();  


  /*4. Deserialize all the data from file */ 
  test_file.open("arithmetic_data_testf.dat", std::ios::in|std::ios::binary);
  ASSERT_EQ(true, test_bin_ser_executor.DeserializeFromDataFile(test_file),"Deserialize fails");
  std::cout << "Check Point 5" << std::endl;

  /*5. Dequeue the arithmetic value */
  bool   d_bool;
  char   d_char;
  int    d_int;
  float  d_float;
  double d_double;

  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(d_bool), "Dequeue bool val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(d_char), "Dequeue bool val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(d_int), "Dequeue bool val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(d_float), "Dequeue bool val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(d_double), "Dequeue bool val fails"); 
  std::cout << "Check Point 6" << std::endl;

  /*6. Test the correctness of the value */
  ASSERT_EQ(d_bool, t_bool, "bool val not equal");
  ASSERT_EQ(d_char, t_char, "char val not equal");
  ASSERT_EQ(d_int, t_int, "int val not equal");
  ASSERT_EQ(d_float, t_float, "float val not equal");
  ASSERT_EQ(d_double, t_double, "double val not equal");

  std::cout << "PASSED" << std::endl;

  return 0;
}