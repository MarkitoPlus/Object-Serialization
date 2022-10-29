8# Object-Serialization
面向对象程序设计期末大作业 二进制序列化

一、支持的数据类型(可参考./include/type.h)
/************************************************/
/  bool                                          /
/  char                                          /
/  int                                           /
/  float                                         /
/  double                                        / 
/  std::string                                   /
/  std::vector                                   /
/  std::list                                     /
/  std::map                                      / 
/  std::set                                      /   
/ std::pair                                      / 
/  用户自定义类型(详见三、用户自定义类使用接口说明) /
/************************************************/


二、BinSerExecutor使用说明
1. 需首先实例化BinSerExecutor

2. 共提供四个方法:
  Enqueue(T val)                        将数据传入BinSerExecutor
  Dequeue(T& val)                       从BinSerExecutor传出数据
  SerializeToDataFile(fstream& fs)      将BinSerExecutor内部所有数据二进制序列化到.dat文件
  DeserializeFromDataFile(fstream& fs)  从二进制.dat文件中接收数据传入BinSerExecutor

3. 例子：
void Example_BinSerExecutor(fstream& fs)
{
  BinSerExecutor test_bse();

  /*Your data*/
  int data1 = 100;
  float data2 = 99.99;

  /*Enqueue data*/
  test_bse.Enqueue(data1);
  test_bse.Enqueue(data2);

  /*Serialize Data*/
  test_bse.SerializeToDataFile(fs);

  /*Reset the cursor of the file since DeserializeFromDataFile()read data from its current cursor*/

  /*Deserialize Data*/
  test_bse.DeserializeFromDataFile(fs);

  int t_data1;
  float t_data2;
  test_bse.Dequeue(t_data1); //(t_data1 == data1) = true  
  test_bse.Dequeue(t_data2); //(t_data2 == data2) = true
}

三、用户自定义类使用接口说明：
1. 当用户自定义了一个数据结构需要进行序列化时，首先需要public 继承接口类 IBinSerUserDefinedType
class User_Data: public IBinSerUserDefinedType{
  /**/
}

2. 在用户自定义的结构体内添加一段含参数宏,参数为需要进行序列化和反序列化的变量名，相互之间用'+'分割(内部使用运算符重载识别，注意首个变量前不需添加'+')
class User_Data: public IBinSerUserDefinedType{
  public:
    UserDefinedSerDataParameter(x+y+z) //请在public下写入该宏,其含义为需要序列化x y z
  public:
    int x;
    double y;
    std::vector<char> z; 
}

3.BinSerExecutor对于用户自定义类型的调用与上述处理普通数据一致


四、测试文件与命令：
1.普通算数数据(is_arithmetic = true)测试
  位置: ./src/test/arithmetic_data_test
  编译命令: g++ arithmetic_data_test.cpp ../databuffer.cpp -I '../include/' -o 'arithmetic_data_test.exe'
  执行命令: ./arithmetic_data_test

2.容器类数据 
  位置: ./src/test/vector_data_test
  编译命令: g++ vector_data_test.cpp databuffer.cpp -I './include/' -o 'vector_data_test.exe'
  执行命令: ./vector_data_test.exe

3.用户自定义类数据
  位置：./src/test/user_defined_type_data_test
  编译命令：g++ user_defined_type_data_test.cpp ../databuffer.cpp -I '../include/' -o 'user_defined_type_data_test.exe'
  执行命令：./user_defined_type_data_test

