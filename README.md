# Object-Serialization
面向对象程序设计期末大作业 二进制序列化与XML序列化



测试方法与命令：
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