#include "BinarySerialization.h"

int main(){
  std::cout << "Check Point 0" << std::endl;
  /*0. Create test file */
  std::fstream test_file;
  test_file.open("vector_data_testf.dat", std::ios::out|std::ios::binary|std::ios::trunc);
  if(!test_file.is_open()) {
    std::cout << "Fail to open .dat file ";
  }
  std::cout << "Check Point 1" << std::endl;

  /*1. Create an executor object of for test */
  test_file.seekp(0, std::ios::beg);
  mkbs::BinSerExecutor test_bin_ser_executor;
  std::cout << "Check Point 2" << std::endl;

  /*2. Enqueue the arithmetic value */
  std::string              s_string = "Markito";
  std::vector<int>         s_vector;
  for(int i = 0; i<10; i++) s_vector.emplace_back(i);
  std::list<int>           s_list;
  for(int i = 0; i<10; i++) s_list.push_back(i);
  std::set<int>            s_set;
  for(int i = 0; i<10; i++) s_set.insert(i);
  std::pair<float, double> s_pair;
  s_pair.first = 99.9; s_pair.second = 100.0001;
  std::map<int, char>      s_map;
  for(int i = 0; i<10; i++) {
    std::pair<int, char> tmp_p(i, 'a'+i);
    s_map.insert(tmp_p);
  }
  std::vector<std::pair<float, double>> complicated_s_vector;
  complicated_s_vector.emplace_back(s_pair);

  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_string), "Enqueue string val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_vector), "Enqueue vector val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_list), "Enqueue list val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_set), "Enqueue set val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_pair), "Enqueue pair val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(s_map), "Enqueue map val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Enqueue(complicated_s_vector), "Enqueue map val fails");
  std::cout << "Check Point 3" << std::endl;

  /*3. Serialize all the data into file */
  ASSERT_EQ(true, test_bin_ser_executor.SerializeToDataFile(test_file), "Serialize fails");
  std::cout << "Check Point 4" << std::endl;
  test_file.close();  


  /*4. Deserialize all the data from file */ 
  test_file.open("vector_data_testf.dat", std::ios::in|std::ios::binary);
  ASSERT_EQ(true, test_bin_ser_executor.DeserializeFromDataFile(test_file),"Deserialize fails");
  std::cout << "Check Point 5" << std::endl;

  /*5. Dequeue the arithmetic value */
  std::string              t_string;
  std::vector<int>         t_vector;
  std::list<int>           t_list;
  std::set<int>            t_set;
  std::pair<float, double> t_pair;
  std::map<int, char>      t_map;
  std::vector<std::pair<float, double>> complicated_t_vector;

  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_string), "Dequeue string val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_vector), "Dequeue vector val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_list), "Dequeue list val fails");
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_set), "Dequeue set val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_pair), "Dequeue  val fails");  
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(t_map), "Dequeue map val fails"); 
  ASSERT_EQ(true, test_bin_ser_executor.Dequeue(complicated_t_vector), "Dequeue map val fails"); 
  std::cout << "Check Point 6" << std::endl;

  /*6. Test the correctness of the value */
  ASSERT_EQ(s_string, t_string, "string val not equal");
  ASSERT_EQ(s_vector, t_vector, "vector val not equal");
  ASSERT_EQ(s_list, t_list, "list val not equal");
  ASSERT_EQ(s_set, t_set, "set val not equal");
  ASSERT_EQ(s_pair, t_pair, "pair val not equal");
  ASSERT_EQ(s_map, t_map, "map val not equal");
  ASSERT_EQ(complicated_s_vector, complicated_t_vector, "map val not equal");

  std::cout << "PASSED" << std::endl;

  return 0;
}