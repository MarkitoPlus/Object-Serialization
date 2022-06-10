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
#include "databuffer.h"
#include "type.h"
#include "macro.h"

namespace mkbs{
/********************************************************************************/
/*                                  BinSerData                                  */
/********************************************************************************/

/* Interface for user defined type, define here for compiling reason. */
/* More details please move to Devlog.md 6/8 */
class BinSerUserDefinedTypeData;
class IBinSerUserDefinedType{
public:
  virtual bool WriteInData(BinSerUserDefinedTypeData* bsudt_data) = 0;

  virtual bool ReadOutData(const BinSerUserDefinedTypeData* bsudt_data) = 0;
};

/* Basic binaray serialization class providing interface for executor */
class BinSerData{
public:
  explicit BinSerData() = delete;
  
  /**
   * @brief Constructor to set type_id
   * 
   * @param type_id 
   */
  explicit BinSerData(TypeId type_id): type_id_(type_id){
  }

  /**
   * @brief Define a type that can be used in executor  
   * 
   * @note For the detailed definition, please move to the inlcude/macro.h
   */
  /*bool type */
  BinSerTypeI(bool)
  BinSerTypeO(bool)

  /*char type */
  BinSerTypeI(char)
  BinSerTypeO(char)

  /*int type */
  BinSerTypeI(int)
  BinSerTypeO(int)

  /*float type I/O*/
  BinSerTypeI(float)
  BinSerTypeO(float)

  /*double type I/O*/
  BinSerTypeI(double)
  BinSerTypeO(double)

  /*std::string type I/O*/
  BinSerTypeI(std::string)
  BinSerTypeO(std::string)

  /*std::vector type I/O*/
  template<typename T>
  BinSerTypeI(std::vector<T>)
  template<typename T>
  BinSerTypeO(std::vector<T>)

  /*std::pair type I/O*/
  template<typename T1, typename T2>
  BinSerTypeI(std::pair<T1 COMMA T2>)
  template<typename T1, typename T2>
  BinSerTypeO(std::pair<T1 COMMA T2>)

  /*std::list type I/O*/
  template<typename T>
  BinSerTypeI(std::list<T>)
  template<typename T>
  BinSerTypeO(std::list<T>)

  /*std::set type I/O*/
  template<typename T>
  BinSerTypeI(std::set<T>)
  template<typename T>
  BinSerTypeO(std::set<T>)

  /*std::map type I/O*/
  template<typename T1, typename T2>
  BinSerTypeI(std::map<T1 COMMA T2>)
  template<typename T1, typename T2>
  BinSerTypeO(std::map<T1 COMMA T2>)

  /*user defined type I/O*/
  static BinSerData* GetInstance(IBinSerUserDefinedType* val);
  bool ReadData(IBinSerUserDefinedType* val);

  /**
   * @brief Serialize Data type into buffer
   * 
   */
  bool SerializeTypeToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  /**
   * @brief Deserialize Data type from buffer
   * 
   */

  static TypeId DeserializeTypeFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);

  /**
   * @brief The interface for derived class to do serialize job
   * 
   */
  virtual bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  /**
   * @brief The interface for derived class to do deserialize job
   * 
   */
  virtual bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);

protected:
  TypeId type_id_; // The type f the data, more details in type.h
};

/********************************************************************************/
/*                              BinSerArithmeticData                            */
/********************************************************************************/
/*Derived binary serialization class implementing arithmetic data structure*/
class BinSerArithmeticData: public BinSerData{
public:
  explicit BinSerArithmeticData() = delete;

  /**
   * @brief Construct a new Bin Ser Arithmetic Data object
   * 
   * @tparam T val The arithmetic value given by user
   * 
   */
  template<typename T>
  explicit BinSerArithmeticData(TypeId type_id, const T val= 0);

  /**
   * @brief Get the Arithmetic Data object
   * 
   * @tparam T val The arithmetic value given by user
   * 
   */
  template<typename T>
  bool GetArithmeticData(T& val);

  /**
   * @brief Serialize the actual data to buffer
   * 
   */
  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  /**
   * @brief Deserialize data from buffer
   * 
   */
  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);
  
private:
  U_Arithmetic_Data arithmetic_data_;  
};

/********************************************************************************/
/*                          BinSerContainerDataHeader                           */
/********************************************************************************/
class BinSerContainerData;
class BinSerContainerDataHeader{

friend class BinSerContainerData;

public:
  explicit BinSerContainerDataHeader(int size, ContainerTypeId container_type_id);

  bool CheckMagicNum(const int check_magic_num);

  bool SerializeBSCHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeBSCHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

private:
  static const int bsch_magic_num_  = BSCH_MAGIC_NUM;

  int size_; //The size of the elements in container, always equal to container.GetSize()

  ContainerTypeId container_type_id_; //The type id of the container
};

/********************************************************************************/
/*                              BinSerContainerData                             */
/********************************************************************************/
/*Derived binary serialization class implementing Vector data structure*/
class BinSerContainerData: public BinSerData{
public:
  explicit BinSerContainerData() = delete;

  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::string& val);

  template<typename T>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::vector<T>& val);

  template<typename T>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::list<T>& val);

  template<typename T>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::set<T>& val);

  template<typename T1, typename T2>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::pair<T1, T2>& val);

  template<typename T1, typename T2>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::map<T1, T2>& val);

  bool GetContainerData(std::string& val);

  template<typename T>
  bool GetContainerData(std::vector<T>& val);

  template<typename T>
  bool GetContainerData(std::list<T>& val);

  template<typename T>
  bool GetContainerData(std::set<T>& val);

  template<typename T1, typename T2>
  bool GetContainerData(std::pair<T1, T2>& val);

  template<typename T1, typename T2>
  bool GetContainerData(std::map<T1, T2>& val);

  /**
   * @brief Serialize the data into buffer
   * 
   */
  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  /**
   * @brief Deserialize data from buffer
   * 
   */
  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer);

private:
  BinSerContainerDataHeader* bin_ser_container_data_header_;

  std::vector<BinSerData*> bin_ser_container_data_;
};

/********************************************************************************/
/*                          BinSerUserDefinedTypeDataHeader                     */
/********************************************************************************/
class BinSerUserDefinedTypeData;
class BinSerUserDefinedTypeDataHeader{

friend class BinSerUserDefinedTypeData;

public:
  explicit BinSerUserDefinedTypeDataHeader(int size);

  bool CheckMagicNum(const int check_magic_num);

  bool SerializeBSUDTHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

  bool DeserializeBSUDTHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer);

private:
  const static int bsudt_magic_num_ = BSUDT_MAGIC_NUM;

  int size_;
};


/********************************************************************************/
/*                          BinSerUserDefinedTypeData                           */
/********************************************************************************/
class BinSerUserDefinedTypeData: public BinSerData{

friend class BSUDTWriteExecutor;

friend class BSUDTReadExecutor;

public:
  explicit BinSerUserDefinedTypeData() = delete;

  explicit BinSerUserDefinedTypeData(TypeId type_id);

  bool SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer) ;

  bool DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer) ;

private:
  BinSerUserDefinedTypeDataHeader* bsudt_data_header_;

  std::vector<BinSerData*> bsudt_data_;

  int offset_;
};

/********************************************************************************/
/*                          BinSerUserDefinedI/O Executor                       */
/********************************************************************************/
class BSUDTWriteExecutor{
public:
  explicit BSUDTWriteExecutor() = delete;

  explicit BSUDTWriteExecutor(BinSerUserDefinedTypeData* bsudt);

  template<typename T>
  BSUDTWriteExecutor& operator+(const T& val);

private:
  BinSerUserDefinedTypeData* bsudt_;
};

class BSUDTReadExecutor{
public:
  explicit BSUDTReadExecutor() = delete;

  explicit BSUDTReadExecutor(BinSerUserDefinedTypeData* bsudt);

  template<typename T> 
  BSUDTReadExecutor& operator+(T& val);

private:
  BinSerUserDefinedTypeData* bsudt_;  
};


/********************************************************************************/
/*                           BinSerExecutorHeader                               */
/********************************************************************************/
class BinSerExecutor;
/* The header of the executor */
class BinSerExecutorHeader{

friend class BinSerExecutor;

public:
  explicit BinSerExecutorHeader();

  bool CheckMagicNum(const int check_magic_num);

  /*  The format of the header serialization as below: (total 12 bytes)
   *        4           4          4
   *  | magic_num |   size_   |  n_byte_  |
   */
  bool SerializeBSEHeaderToDataFile(std::fstream& fs);

  bool DeserializeBSEHeaderFromDataFile(std::fstream& fs);

private:
  const static int bseh_magic_num_ = BSEH_MAGIC_NUM; // help confirm the BinSerExecutor

  int size_; // The total number of data in BinSerExecutor, used for BinSerExecutor

  int n_byte_; // the total number of bytes, used for databuffer when deserialization

  int offset_; //the current index of BinSerData to be read
};


/********************************************************************************/
/*                               BinSerExecutor                                 */
/********************************************************************************/
/* The Executor exposed to user */
class BinSerExecutor{
public:
  /**
   * @brief Construct a new Bin Ser Executor object
   * 
   */
  explicit BinSerExecutor();

  /*  The format of the serialization as below
   *  
   *          12               N
   *  |   BSE_Header  |      Data     |
   */

  /**
   * @brief Serialize the whole data including every header and every actual data to .data file  
   * 
   * @param fs the .data file
   * 
   * @return true Serialize successfully
   * 
   * @return false Faild to serialize
   * 
   * @note After serilaizing the whole data, all the info in executor will be released.
   */
  bool SerializeToDataFile(std::fstream& fs);

  /**
   * @brief Deserialize the whole data from .data file
   * 
   * @param fs the .data file
   * 
   * @return true Deserialize successfully
   * 
   * @return false Failed to deserialize
   */
  bool DeserializeFromDataFile(std::fstream& fs);

  /**
   * @brief Enqueue the value
   *  
   */
  template<typename T>
  bool Enqueue(T val){
     binary_serialization_data_.emplace_back(BinSerData::GetInstance(val));
    return true;
  }

  /**
   * @brief Dequeue the data from BinSerExecutor to value
   * 
   * @param val 
   * 
   * @return true Dequeue successfully 
   * 
   * @return false Fail to dequeue
   */
  template<typename T>
  bool Dequeue(T& val){
    if(binary_serialization_executor_header_->offset_ >= binary_serialization_data_.size()) return false; 
    else if(!binary_serialization_data_.at(binary_serialization_executor_header_->offset_)->ReadData(val)) return false;
    binary_serialization_executor_header_->offset_++;
    return true;
  }

private:
  BinSerExecutorHeader*     binary_serialization_executor_header_; //The header of the executor
 
  std::vector<BinSerData*>  binary_serialization_data_; //The vector containing the actual data

  mkbf::BinaryDataBuffer*   binary_data_buffer_; //The buffer containing the binary data
};

}























using namespace mkbs;
#include "BinarySerialization.h"

using namespace mkbs;

/********************************************************************************/
/*                                  BinSerData                                  */
/********************************************************************************/
BinSerArithmeticDataI(bool, TypeId::bool_t)
BinSerArithmeticDataO(bool, TypeId::bool_t)

BinSerArithmeticDataI(char, TypeId::char_t)
BinSerArithmeticDataO(char, TypeId::char_t)

BinSerArithmeticDataI(int, TypeId::int_t)
BinSerArithmeticDataO(int, TypeId::int_t)

BinSerArithmeticDataI(float, TypeId::float_t)
BinSerArithmeticDataO(float, TypeId::float_t)

BinSerArithmeticDataI(double, TypeId::double_t)
BinSerArithmeticDataO(double, TypeId::double_t)

BinSerContainerDataI(std::string, TypeId::string_t, ContainerTypeId::ct_single_t)
BinSerContainerDataO(std::string, TypeId::string_t)

template<typename T>
BinSerContainerDataI(std::vector<T>, TypeId::std_vector_t, ContainerTypeId::ct_single_t)
template<typename T>
BinSerContainerDataO(std::vector<T>, TypeId::std_vector_t)

template<typename T1, typename T2>
BinSerContainerDataI(std::pair<T1 COMMA T2>, TypeId::std_pair_t, ContainerTypeId::ct_double_t)
template<typename T1, typename T2>
BinSerContainerDataO(std::pair<T1 COMMA T2>, TypeId::std_pair_t)

template<typename T>
BinSerContainerDataI(std::list<T>, TypeId::std_list_t, ContainerTypeId::ct_single_t)
template<typename T>
BinSerContainerDataO(std::list<T>, TypeId::std_list_t)

template<typename T>
BinSerContainerDataI(std::set<T>, TypeId::std_set_t, ContainerTypeId::ct_single_t)
template<typename T>
BinSerContainerDataO(std::set<T>, TypeId::std_set_t)

template<typename T1, typename T2>
BinSerContainerDataI(std::map<T1 COMMA T2>, TypeId::std_map_t, ContainerTypeId::ct_double_t)
template<typename T1, typename T2>
BinSerContainerDataO(std::map<T1 COMMA T2>, TypeId::std_map_t)

BinSerData* BinSerData::GetInstance(IBinSerUserDefinedType* val){
  BinSerUserDefinedTypeData* user_defined_data = new BinSerUserDefinedTypeData(TypeId::user_defined_t);
  val->WriteInData(user_defined_data);
  return user_defined_data;
}
bool BinSerData::ReadData(IBinSerUserDefinedType* val){
  if(type_id_!=TypeId::user_defined_t || !val->ReadOutData(BSUDTPTR(this))) return false;
  return true;
}

bool BinSerData::SerializeTypeToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  binary_data_buffer->WriteData(CPTR(type_id_), sizeof(TypeId));
  return true;
}

TypeId BinSerData::DeserializeTypeFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer){
  TypeId ret_type_id = TypeId::invalid_t;
  binaray_data_buffer->ReadData(CPTR(ret_type_id), sizeof(TypeId));
  return ret_type_id;
}


bool BinSerData::SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  ASSERT(false, "Call a pure function! ");
  return true;
}

bool BinSerData::DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  ASSERT(false, "Call a pure function! ");
  return true;
}

/********************************************************************************/
/*                              BinSerArithmeticData                            */
/********************************************************************************/
template<typename T>
BinSerArithmeticData::BinSerArithmeticData(TypeId type_id, const T val): BinSerData(type_id){
  switch (type_id_)
  {
  case TypeId::bool_t:
    arithmetic_data_.bool_val_ = val;
    break;
  case TypeId::char_t:
    arithmetic_data_.char_val_ = val;
    break;
  case TypeId::int_t:
    arithmetic_data_.int_val_ = val;
    break;
  case TypeId::float_t:
    arithmetic_data_.float_val_ = val;
    break;
  case TypeId::double_t:
    arithmetic_data_.double_val_ = val;
    break;
  default:
    ASSERT(false, "Not Arithmeticdata type!");
    break;
  }
}

template<typename T>
bool BinSerArithmeticData::GetArithmeticData(T& val){
  switch (type_id_)
  {
  case TypeId::bool_t:
    val = arithmetic_data_.bool_val_;
    break;
  case TypeId::char_t:
     val = arithmetic_data_.char_val_;
    break;
  case TypeId::int_t:
    val = arithmetic_data_.int_val_;
    break;
  case TypeId::float_t:
    val = arithmetic_data_.float_val_;
    break;
  case TypeId::double_t:
    val = arithmetic_data_.double_val_ ;
    break;
  default:
    return false;
    /*If type not matches, the problem might occur from user, just return false */
    // ASSERT(false, "Not Arithmeticdata type!"); 
    break;
  }
  return true;
}

bool BinSerArithmeticData::SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  SerializeTypeToBuf(binary_data_buffer);

  switch (type_id_)
  {
  case TypeId::bool_t:
    binary_data_buffer->WriteData(CPTR(arithmetic_data_.bool_val_), sizeof(bool));
    break;
  case TypeId::char_t:
    binary_data_buffer->WriteData(CPTR(arithmetic_data_.bool_val_), sizeof(char));
    break;
  case TypeId::int_t:
    binary_data_buffer->WriteData(CPTR(arithmetic_data_.bool_val_), sizeof(int));
    break;
  case TypeId::float_t:
    binary_data_buffer->WriteData(CPTR(arithmetic_data_.bool_val_), sizeof(float));
    break;
  case TypeId::double_t:
    binary_data_buffer->WriteData(CPTR(arithmetic_data_.bool_val_), sizeof(double));
    break;
  default:
    ASSERT(false, "Not Arithmeticdata type!"); 
  }
  return true;
}

bool BinSerArithmeticData::DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer){
  switch (type_id_)
  {
  case TypeId::bool_t:
    return binaray_data_buffer->ReadData(CPTR(arithmetic_data_.bool_val_), sizeof(bool));
  case TypeId::char_t:
    return binaray_data_buffer->ReadData(CPTR(arithmetic_data_.char_val_), sizeof(char));
  case TypeId::int_t:
    return binaray_data_buffer->ReadData(CPTR(arithmetic_data_.int_val_), sizeof(int));
  case TypeId::float_t:
    return binaray_data_buffer->ReadData(CPTR(arithmetic_data_.float_val_), sizeof(float));
  case TypeId::double_t:
    return binaray_data_buffer->ReadData(CPTR(arithmetic_data_.double_val_), sizeof(double));
  default:
    break;
  }
  return false;
}


/********************************************************************************/
/*                          BinSerContainerDataHeader                           */
/********************************************************************************/
BinSerContainerDataHeader::BinSerContainerDataHeader(int size, ContainerTypeId container_type_id)\
        :size_(size), container_type_id_(container_type_id) {}

bool BinSerContainerDataHeader::CheckMagicNum(const int check_magic_num){
  return (check_magic_num == BinSerContainerDataHeader::bsch_magic_num_);
}

bool BinSerContainerDataHeader::SerializeBSCHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0. Create a temporary char array to store the binary data of the header */
  char* c_bscdh_data = new char[BIN_SER_CONTAINER_HEADER_LENGTH];
  int buf_ofs = 0;

  /*1. Copy the magic number */
  int tmp_magic_number = bsch_magic_num_;
  memcpy(c_bscdh_data+buf_ofs, CPTR(tmp_magic_number), sizeof(int));
  buf_ofs += sizeof(int);

  /*2. Copy the size */
  memcpy(c_bscdh_data+buf_ofs, CPTR(size_), sizeof(int));
  buf_ofs += sizeof(int);

  /*3. Copy the container_id */
  memcpy(c_bscdh_data+buf_ofs, CPTR(container_type_id_), sizeof(ContainerTypeId));
  buf_ofs += sizeof(ContainerTypeId);
  ASSERT(buf_ofs == BIN_SER_CONTAINER_HEADER_LENGTH, "The buf_ofs does not match to the header length!");

  /*4. Clone the data */
  binary_data_buffer->WriteData(c_bscdh_data, buf_ofs);

  /*5. Delete the temporary data */
  delete[] c_bscdh_data;

  return true;
}

bool BinSerContainerDataHeader::DeserializeBSCHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0.  Create a temporary char array to store the binary data of the header from buffer */
  char* c_bscdh_data = new char[BIN_SER_CONTAINER_HEADER_LENGTH];
  int buf_ofs = 0;

  /*1. Clone the data */ 
  binary_data_buffer->ReadData(c_bscdh_data, BIN_SER_CONTAINER_HEADER_LENGTH);

  /*2. Check the magic number */
  int tmp_magic_number;
  memcpy(CPTR(tmp_magic_number), c_bscdh_data+buf_ofs, sizeof(int));
  if(!CheckMagicNum(tmp_magic_number)) return false;
  buf_ofs += sizeof(int);

  /*3. Copy the size_*/
  memcpy(CPTR(size_), c_bscdh_data+buf_ofs,sizeof(int));
  buf_ofs += sizeof(int);

  /*4. Copy the container_id */
  memcpy(CPTR(container_type_id_), c_bscdh_data+buf_ofs, sizeof(ContainerTypeId));
  buf_ofs += sizeof(ContainerTypeId);
  ASSERT(buf_ofs == BIN_SER_CONTAINER_HEADER_LENGTH, "The buf_ofs does not match to the header length!");

  /*5. delete the temporary data */
  delete[] c_bscdh_data;

  return true;
}


/********************************************************************************/
/*                              BinSerContainerData                             */
/********************************************************************************/
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::string& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(val.size(), container_type_id);
  for(int i = 0; i<val.size(); i++){
    char tmp_val = val.at(i);
    bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val));
  } 
}

template<typename T>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::vector<T>& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(val.size(), container_type_id);
  for(int i = 0; i<val.size(); i++){
    T tmp_val = val.at(i);
    bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val));
  } 
}

template<typename T>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::list<T>& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(val.size(), container_type_id);
  for(auto it = val.begin(); it!=val.end() ;it++) {
    T tmp_val = *it;
    bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val));
  }
}

template<typename T>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::set<T>& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(val.size(), container_type_id);
  for(auto it = val.begin(); it!=val.end() ;it++){
    T tmp_val = *it;
    bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val));
  } 
}

template<typename T1, typename T2>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::pair<T1, T2>& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(2, container_type_id);
  T1 tmp_val1 = val.first;
  T2 tmp_val2 = val.second;
  bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val1));
  bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val2));
}

template<typename T1, typename T2>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, std::map<T1, T2>& val): BinSerData(type_id){
  bin_ser_container_data_header_ = new BinSerContainerDataHeader(val.size(), container_type_id);
  for(auto it = val.begin(); it!=val.end() ;it++){
    std::pair<T1, T2> tmp_val;
    tmp_val.first = it->first;
    tmp_val.second = it->second;
    bin_ser_container_data_.emplace_back(BinSerData::GetInstance(tmp_val));
  }
}

bool BinSerContainerData::GetContainerData(std::string& val){
  if(type_id_ != TypeId::string_t) return false;
  char* tmp_s = new char[bin_ser_container_data_.size()];
  for(int i = 0; i<bin_ser_container_data_.size(); i++){
    bin_ser_container_data_.at(i)->ReadData(tmp_s[i]);
  }
  val = tmp_s;
  return true;
}

template<typename T>
bool BinSerContainerData::GetContainerData(std::vector<T>& val){
  if(type_id_ != TypeId::std_vector_t) return false;
  for(int i = 0; i<bin_ser_container_data_.size(); i++){
    T tmp_val;
    bin_ser_container_data_.at(i)->ReadData(tmp_val);
    val.emplace_back(tmp_val);
  }
  return true;
}

template<typename T>
bool BinSerContainerData::GetContainerData(std::list<T>& val){
  if(type_id_ != TypeId::std_list_t) return false;
  for(int i = 0; i<bin_ser_container_data_.size(); i++){
    T tmp_val;
    bin_ser_container_data_.at(i)->ReadData(tmp_val);
    val.push_back(tmp_val);
  }
  return true;
}

template<typename T>
bool BinSerContainerData::GetContainerData(std::set<T>& val){
  if(type_id_ != TypeId::std_set_t) return false;
  for(int i = 0; i<bin_ser_container_data_.size(); i++){
    T tmp_val;
    bin_ser_container_data_.at(i)->ReadData(tmp_val);
    val.insert(tmp_val);
  }
  return true;
}

template<typename T1, typename T2>
bool BinSerContainerData::GetContainerData(std::pair<T1, T2>& val){
  if(type_id_ != TypeId::std_pair_t) return false;
  bin_ser_container_data_.at(0)->ReadData(val.first);
  bin_ser_container_data_.at(1)->ReadData(val.second);
  return true;
}

template<typename T1, typename T2>
bool BinSerContainerData::GetContainerData(std::map<T1, T2>& val){
  if(type_id_ != TypeId::std_map_t) return false;
  for(int i = 0; i<bin_ser_container_data_.size(); i++){
   std::pair<T1, T2> tmp_pair;
   bin_ser_container_data_.at(i)->ReadData(tmp_pair);
   val.insert(tmp_pair);
  }
  return true;
}

bool BinSerContainerData::SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0. Serialize the type to the buffer */
  SerializeTypeToBuf(binary_data_buffer);

  /*1. Serialize the header of the container */
  bin_ser_container_data_header_->SerializeBSCHeaderToBuf(binary_data_buffer);

  /*2. Serialize the actual data of the container */
  for(int i = 0; i<bin_ser_container_data_.size(); i++)
    bin_ser_container_data_.at(i)->SerializeDataToBuf(binary_data_buffer);
  
  return true;
}

bool BinSerContainerData::DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer){
  /*-1 note: before call the function, obviously we have obtained the typeid*/
  /*0. Deserialize the header of the container */
  if(!bin_ser_container_data_header_->DeserializeBSCHeaderFromBuf(binaray_data_buffer)) return false;

  /*1. Deserialize the actual data of the container */
  for(int i = 0; i<bin_ser_container_data_header_->size_; i++){
    DESERIALIZEDATA(deser_data, binaray_data_buffer)
    bin_ser_container_data_.emplace_back(deser_data);
  }
  return true;
}


/********************************************************************************/
/*                          BinSerUserDefinedTypeDataHeader                     */
/********************************************************************************/
BinSerUserDefinedTypeDataHeader::BinSerUserDefinedTypeDataHeader(int size): size_(size){
}


bool BinSerUserDefinedTypeDataHeader::CheckMagicNum(const int check_magic_num){
  return (check_magic_num == BinSerUserDefinedTypeDataHeader::bsudt_magic_num_);
}

bool BinSerUserDefinedTypeDataHeader::SerializeBSUDTHeaderToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0. Copy the magic number */
  int tmp_magic_num = bsudt_magic_num_;
  binary_data_buffer->WriteData(CPTR(tmp_magic_num), sizeof(int));

  /*1. Copy the size_*/
  binary_data_buffer->WriteData(CPTR(size_), sizeof(int));

  return true;
}

bool BinSerUserDefinedTypeDataHeader::DeserializeBSUDTHeaderFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0. Check the magic number */
  int tmp_magic_num;
  binary_data_buffer->ReadData(CPTR(tmp_magic_num), sizeof(int));
  if(!CheckMagicNum(tmp_magic_num)) return false;

  /*1. Deserialize the size_*/
  binary_data_buffer->ReadData(CPTR(size_), sizeof(int));

  return true;
}


/********************************************************************************/
/*                          BinSerUserDefinedTypeData                           */
/********************************************************************************/
BinSerUserDefinedTypeData::BinSerUserDefinedTypeData(TypeId type_id): BinSerData(type_id), bsudt_data_header_(0), offset_(0){
}

bool BinSerUserDefinedTypeData::SerializeDataToBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  /*0. Serialize the type to the buffer */
  SerializeTypeToBuf(binary_data_buffer);

  /*1. Serialize the header of the container */
  bsudt_data_header_->size_ = bsudt_data_.size();
  bsudt_data_header_->SerializeBSUDTHeaderToBuf(binary_data_buffer);

  /*2. Serialize the actual data of the container */
  for(int i = 0; i<bsudt_data_.size(); i++)
    bsudt_data_.at(i)->SerializeDataToBuf(binary_data_buffer);

  return true;
}

bool BinSerUserDefinedTypeData::DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binaray_data_buffer){
  /*-1 note: before call the function, obviously we have obtained the typeid*/
  /*0. Deserialize the header of the container */
  if(!bsudt_data_header_->DeserializeBSUDTHeaderFromBuf(binaray_data_buffer)) return false;

  /*1. Deserialize the actual data of the container */
  for(int i = 0; i<bsudt_data_header_->size_; i++){
    DESERIALIZEDATA(deser_data, binaray_data_buffer)
    bsudt_data_.emplace_back(deser_data);
  }

  return true;
}


/********************************************************************************/
/*                          BinSerUserDefinedI/O Executor                       */
/********************************************************************************/
BSUDTWriteExecutor::BSUDTWriteExecutor(BinSerUserDefinedTypeData* bsudt): bsudt_(bsudt){
}


template<typename T>
BSUDTWriteExecutor& BSUDTWriteExecutor::operator+(const T& val){
  bsudt_->bsudt_data_.emplace_back(BinSerData::GetInstance(val));
  return *this;
}

BSUDTReadExecutor::BSUDTReadExecutor(BinSerUserDefinedTypeData* bsudt): bsudt_(bsudt){
}

template<typename T> 
BSUDTReadExecutor& BSUDTReadExecutor::operator+(T& val){
  bsudt_->bsudt_data_.at(bsudt_->offset_)->ReadData(val);
  bsudt_->offset_++;
  return *this;
}

/********************************************************************************/
/*                           BinSerExecutorHeader                               */
/********************************************************************************/
BinSerExecutorHeader::BinSerExecutorHeader(): size_(0), n_byte_(0), offset_(0){
}

bool BinSerExecutorHeader::CheckMagicNum(const int check_magic_num){
  return (check_magic_num == BinSerExecutorHeader::bseh_magic_num_);
}

bool BinSerExecutorHeader::SerializeBSEHeaderToDataFile(std::fstream& fs){
  /*0. Create a pointer to store the binary data of this header */
  char* c_bseh_data = new char[BIN_SER_HEADER_LENGTH];
  int buf_ofs = 0;

  /*1. Copy the magic number */
  int tmp_magic_num = bseh_magic_num_; //temporary value to store the magic num sice const value can not reinterpret
  memcpy(c_bseh_data+buf_ofs, CPTR(tmp_magic_num ), sizeof(int));
  buf_ofs += sizeof(int);

  /*2. Copy the size_*/
  memcpy(c_bseh_data+buf_ofs, CPTR(size_), sizeof(int));
  buf_ofs += sizeof(int);
  
  /*3. Copy the number of bytes*/
  memcpy(c_bseh_data+buf_ofs, CPTR(n_byte_), sizeof(int));
  buf_ofs += sizeof(int);
  ASSERT(buf_ofs == BIN_SER_HEADER_LENGTH, "The buf_ofs does not match to the header length!");

  /*4. Clone the header to data file*/
  fs.write(c_bseh_data, buf_ofs);

  /*5. Delete the temporary header*/
  delete[] c_bseh_data;

  return true;
}

bool BinSerExecutorHeader::DeserializeBSEHeaderFromDataFile(std::fstream& fs){
  /*0. Create a pointer to receive the binary data from file */
  char* c_bseh_data = new char[BIN_SER_HEADER_LENGTH];
  int buf_ofs = 0;

  /*1. Read the data from file */
  fs.read(c_bseh_data, BIN_SER_HEADER_LENGTH);

  /*2. Copy the magic num and check it */
  int tmp_magic_num;
  memcpy(CPTR(tmp_magic_num), c_bseh_data+buf_ofs, sizeof(int));
  if(!CheckMagicNum(tmp_magic_num)) return false; //wrong file, actully we can use a more serious check as the comment below
  // ASSERT(CheckMagicNum(tmp_magic_num), "Can not recognize valid magic num!");  
  buf_ofs += sizeof(int);

  /*3. Copy the size_ */
  memcpy(CPTR(size_), c_bseh_data+buf_ofs, sizeof(int));
  buf_ofs += sizeof(int);

  /*4. Copy the number of bytes */
  memcpy(CPTR(n_byte_), c_bseh_data+buf_ofs, sizeof(int));
  buf_ofs += sizeof(int);
  ASSERT(buf_ofs == BIN_SER_HEADER_LENGTH, "The buf_ofs does not match to the header length!");

  return true;
}


/********************************************************************************/
/*                               BinSerExecutor                                 */
/********************************************************************************/
BinSerExecutor::BinSerExecutor(){
  binary_serialization_executor_header_ = new BinSerExecutorHeader();
  binary_data_buffer_ = new mkbf::BinaryDataBuffer();
}

bool BinSerExecutor::SerializeToDataFile(std::fstream& fs){
  /*0. Serialize all the BinSerData into buffer and clear them*/
  for(int i = 0; i<binary_serialization_data_.size(); i++){
    binary_serialization_data_.at(i)->SerializeDataToBuf(binary_data_buffer_);
    delete binary_serialization_data_.at(i);
  }

  /*1. Update the BinSerExecutor header */
  binary_serialization_executor_header_->size_ = binary_serialization_data_.size();
  binary_serialization_executor_header_->n_byte_ = binary_data_buffer_->GetSize();

  /*2. Serialize the BinSerExecutor header */
  binary_serialization_executor_header_->SerializeBSEHeaderToDataFile(fs);

  /*3. Serialize the all the data into file */
  binary_data_buffer_->WriteDataToDataFile(fs);

  /*4. Clear the buffer and header*/
  binary_data_buffer_->Clear();
  binary_serialization_executor_header_->offset_ = 0;
  binary_serialization_data_.clear();

  return true;
}

bool BinSerExecutor::DeserializeFromDataFile(std::fstream& fs){
  /*0. Deserialize the header from data file and check the magic number */
  if(!binary_serialization_executor_header_->DeserializeBSEHeaderFromDataFile(fs)) return false;
  
  /*1. Read the acutal data from data file to buffer*/
  binary_data_buffer_->ReadDataFromDataFile(fs, binary_serialization_executor_header_->n_byte_);

  /*2. Deserialize the actual data*/
  for(int i = 0; i<binary_serialization_executor_header_->size_; i++){
    DESERIALIZEDATA(bin_deser_data, binary_data_buffer_)
    binary_serialization_data_.emplace_back(bin_deser_data);
  }  

  /*3. Clear the buffer */
  binary_data_buffer_->Clear();

  return true;
}
//template void SwapType<int>(int &a, int &b);

#endif