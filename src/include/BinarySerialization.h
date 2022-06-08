#ifndef BINARARY_SERIALIZATION_H
#define BINARARY_SERIALIZATION_H

#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <iostream>
#include "type.h"
#include "databuffer.h"

namespace mkbs{

class BinSerExecutor{
public:
  /*default constructor*/
  explicit BinSerExecutor();


  /********************************************************************************/
  /*          Serialization and Deserialization between memory and .data file     */                 
  /********************************************************************************/
  
  /*  The format of the serialization as below
   *  
   *          12               N
   *  |   BSE_Header  |      Data     |
   */
  bool SerializeToDataFile(std::fstream& fs);

  bool DeserializeFromDataFile(std::fstream& fs);

  /********************************************************************************/
  /*                              Data I/O Interface                              */
  /********************************************************************************/
  
  template<typename T>
  bool Enqueue(const T val);

  template<typename T>
  bool Dequeue(T& val);

private:
  BinSerExecutorHeader*     binary_serialization_executor_header_;

  std::vector<BinSerData*>  binary_serialization_data_;

  mkbf::BinaryDataBuffer*   binary_data_buffer_; 
};

class BinSerExecutorHeader{

friend class BinSerExecutor;

public:
  bool CheckMagicNum(const int check_magic_num);

  /*  The format of the header serialization as below: (total 12 bytes)
   *        4           4          4
   *  | magic_num |   size_   |  n_byte_  |
   */
  bool SerializeBSEHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeBSEHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

private:
  const static int bseh_magic_num_ = BSEH_MAGIC_NUM; // help confirm the BinSerExecutor

  int size_; // The total number of data in BinSerExecutor, used for BinSerExecutor

  int n_byte_; // the total number of bytes, used for databuffer when deserialization
};

/*Basic binaray serialization class providing interface */
class BinSerData{
public:
  explicit BinSerData() = delete;

  explicit BinSerData(TypeId type_id);

  bool SerializeTypeToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeTypeFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  /********************************************************************************/
  /*                              Data I/O operation                              */
  /********************************************************************************/
  
  /*bool type I/O*/
  static BinSerData* GetInstance(const bool& bool_val);

  bool ReadData(bool& bool_val);

  /*char type I/O*/
  static BinSerData* GetInstance(const char& char_val);

  bool ReadData(char& char_val);

  /*int type I/O*/
  static BinSerData* GetInstance(const int& int_val);

  bool ReadData(int& int_val);

  /*float type I/O*/
  static BinSerData* GetInstance(const float& float_val);

  bool ReadData(float& float_val);

  /*double type I/O*/
  static BinSerData* GetInstance(const double& double_val);

  bool ReadData(double& double_val);

  /*std::string type I/O*/
  static BinSerData* GetInstance(const std::string& std_string_val);

  bool ReadData(std::string& std_string_val);

  /*std::vector type I/O*/
  template<typename T1, typename T2>
  static BinSerData* GetInstance(const std::vector<T1, T2>& std_vector_val);

  template<typename T1, typename T2>
  bool ReadData(std::vector<T1, T2>& std_vector_val);

  /*std::pair type I/O*/
  template<typename T1, typename T2>
  static BinSerData* GetInstance(const std::pair<T1, T2>& std_pair_val);
  
  template<typename T1, typename T2>
  bool ReadData(std::pair<T1, T2>& std_pair_val);

  /*std::list type I/O*/
  template<typename T>
  static BinSerData* GetInstance(const std::list<T>& std_list_val);

  template<typename T>
  bool ReadData(std::list<T>& std_list_val);

  /*std::set type I/O*/
  template<typename T>
  static BinSerData* GetInstance(const std::set<T>& std_set_val);

  template<typename T>
  bool ReadData(std::set<T>& std_set_val);

  /*std::map type I/O*/
  template<typename T1, typename T2>
  static BinSerData* GetInstance(const std::map<T1, T2>& std_map_val);

  template<typename T1, typename T2>
  bool ReadData(std::map<T1, T2>& std_map_val);

  /*user defined type I/O*/
  static BinSerData* GetInstance(const IBinSerUserDefinedType& user_defined_val);

  template<typename T>
  bool ReadData(IBinSerUserDefinedType& user_defined_val);
  
  virtual bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer) = 0;

  virtual bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer) = 0;

private:
  TypeId type_id_;
};

/*Derived binary serialization class implementing arithmetic data structure*/
class BinSerArithmeticData: public BinSerData{
public:
  explicit BinSerArithmeticData() = delete;

  template<typename T>
  explicit BinSerArithmeticData(TypeId type_id, const T val);

  template<typename T>
  bool GetArithmeticData(T& val);

  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);
  
private:
  union arithmetic_data_{
    bool    bool_val_;
    char    char_val_;
    int     int_val_;
    float   float_val_;
    double  double_val_;
  };
};

/*Derived binary serialization class implementing Vector data structure*/
class BinSerContainerData: public BinSerData{
public:
  explicit BinSerContainerData() = delete;

  template<typename T>
  explicit BinSerContainerData(TypeId type_id, const T& val);

  template<typename T>
  bool GetContainerData(T& val);

  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);

private:
  BinSerContainerDataHeader* bin_ser_container_data_header_;

  std::vector<BinSerData*> bin_ser_container_data_;
};

class BinSerContainerDataHeader{

friend class BinSerContainerData;

public:
  bool CheckMagicNum(const int check_magic_num);

  bool SerializeBSCHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeBSCHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

private:
  static const int bsch_magic_num_  = BSCH_MAGIC_NUM;

  int size_; //The size of the elements in container, always equal to container.GetSize();
};

class BinSerUserDefinedTypeData: public BinSerData{

friend class BSUDTWriteExecutor;

friend class BSUDTReadExecutor;

public:
  explicit BinSerUserDefinedTypeData() = delete;

  template<typename T>
  explicit BinSerUserDefinedTypeData(TypeId type_id);

  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer) ;

  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer) ;

private:
  BinSerUserDefinedTypeDataHeader* bsudt_data_header_;

  std::vector<BinSerData*> bsudt_data_;
};

class BinSerUserDefinedTypeDataHeader{

friend class BinSerUserDefinedTypeData;

public:
  bool CheckMagicNum(const int check_magic_num);

  bool SerializeBSUDTHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeBSUDTHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

private:
  static const int bsudt_magic_num_ = BSUDT_MAGIC_NUM;
};

/*Interface for user defined type*/
class IBinSerUserDefinedType{
public:
  virtual bool WriteInData(BinSerUserDefinedTypeData& bsudt_data) = 0;

  virtual bool ReadOutData(const BinSerUserDefinedTypeData& bsudt_data) = 0;
};

class BSUDTWriteExecutor{
public:
  explicit BSUDTWriteExecutor() = delete;

  explicit BSUDTWriteExecutor(BinSerUserDefinedTypeData* bsudt_data);

  template<typename T>
  BSUDTWriteExecutor& operator+(const T& val);

private:
  BinSerUserDefinedTypeData* bsudt_data_;
};

class BSUDTReadExecutor{
public:
  explicit BSUDTReadExecutor() = delete;

  explicit BSUDTReadExecutor(BinSerUserDefinedTypeData* bsudt_data);

  template<typename T> 
  BSUDTReadExecutor& operator+(T& val);

private:
  BinSerUserDefinedTypeData* bsudt_data_;  
};

}

#endif