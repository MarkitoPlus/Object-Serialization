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
  explicit BinSerData(TypeId type_id);

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

  /**
   * @brief Write single type container
   * 
   */
  template<typename T>
  void WriteSingleType(const T& val);

  /**
   * @brief Write double type container
   * 
   */
  template<typename T>
  void WriteDoubleType(const T& val);
  /**
   * @brief Construct a new Bin Ser Container Data object
   * 
   * @param type_id the type_id of the container
   * 
   * @param container_type_id the contanier_type_id
   * 
   * @param val the value given by user
   */
  
  template<typename T>
  explicit BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, const T& val);

  /**
   * @brief Read single type container data 
   * 
   */
  template<typename T>
  void ReadSingleType(T& val);

  /**
   * @brief Read double type container data 
   * 
   */
  template<typename T>
  void ReadDoubleType(T& val);

  /**
   * @brief Get the Container Data object
   * 
   * @tparam T 
   * @param val 
   * @return true 
   * @return false 
   */
  template<typename T>
  bool GetContainerData(T& val);

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
  bool Enqueue(const T val);

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
  bool Dequeue(T& val);

private:
  BinSerExecutorHeader*     binary_serialization_executor_header_; //The header of the executor
 
  std::vector<BinSerData*>  binary_serialization_data_; //The vector containing the actual data

  mkbf::BinaryDataBuffer*   binary_data_buffer_; //The buffer containing the binary data
};

}

#endif