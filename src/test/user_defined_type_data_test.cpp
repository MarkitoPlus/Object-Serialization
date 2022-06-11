#include "BinarySerialization.h"

int main(){
  std::cout << "Check Point 0" << std::endl;
  /*0. Create test file */
  std::fstream test_file;
  test_file.open("user_defined_type_data_testf.dat", std::ios::out|std::ios::binary|std::ios::trunc);
  if(!test_file.is_open()) {
    std::cout << "Fail to open .dat file ";
  }
  std::cout << "Check Point 1" << std::endl;

  /*1. Create an executor object of for test */
  test_file.seekp(0, std::ios::beg);
  mkbs::BinSerExecutor test_bin_ser_executor;
  std::cout << "Check Point 2" << std::endl;

  /*2. Enqueue the arithmetic value */
  class User_Defined: public mkbs::IBinSerUserDefinedType{
    public:
      UserDefinedSerDataParameter(x+y+z)
    public:
      int x;
      double y;
      std::vector<char> z; 
  };

  User_Defined* t_user_defined = new User_Defined();

  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(t_user_defined), "Enqueue bool val fails");
  std::cout << "Check Point 3" << std::endl;

  /*3. Serialize all the data into file */
  ASSERT_EQ(true, test_bin_ser_executor.SerializeToDataFile(test_file), "Serialize fails");
  std::cout << "Check Point 4" << std::endl;
  test_file.close();  


  /*4. Deserialize all the data from file */ 
  test_file.open("user_defined_type_data_testf.dat", std::ios::in|std::ios::binary);
  ASSERT_EQ(true, test_bin_ser_executor.DeserializeFromDataFile(test_file),"Deserialize fails");
  std::cout << "Check Point 5" << std::endl;

  /*5. Dequeue the arithmetic value */
  User_Defined* s_user_defined = new User_Defined();

  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(s_user_defined), "Dequeue bool val fails"); 
  std::cout << "Check Point 6" << std::endl;

  /*6. Test the correctness of the value */
  ASSERT_EQ(t_user_defined->x, s_user_defined->x, "user defined type value not equal");
  ASSERT_EQ(t_user_defined->y, s_user_defined->y, "user defined type value not equal");
  ASSERT_EQ(t_user_defined->z, s_user_defined->z, "int val not equal");

  std::cout << "PASSED" << std::endl;

  return 0;
}