#ifndef MACRO_H
#define MACRO_H

/********************************************************************************/
/*                                 Common MACRO                                 */
/********************************************************************************/
#define ASSERT(expr, message) assert((expr) && (message))

#define ASSERT_EQ(expr1, expr2, message) ASSERT((expr1) == (expr2), message)

#define ASSERT_NE(expr1, expr2, message) ASSERT((expr1) != (expr2), message)

#define COMMA ,

#define CPTR(val) reinterpret_cast<char*>(&val)

#define BSAPTR(val) reinterpret_cast<BinSerArithmeticData*>(val)

#define BSCPTR(val) reinterpret_cast<BinSerContainerData*>(val)

#define BSUDTPTR(val) reinterpret_cast<BinSerUserDefinedTypeData*>(val)


/********************************************************************************/
/*                              Magic Number MACRO                              */
/********************************************************************************/
const int BSEH_MAGIC_NUM = 838981;

const int BSCH_MAGIC_NUM = 838982;

const int BSUDT_MAGIC_NUM = 838983;

/********************************************************************************/
/*                           BinSerData I/O method MACRO                        */
/********************************************************************************/
#define BinSerTypeI(type_parm)  static BinSerData* GetInstance(type_parm& val);  
                         
#define BinSerTypeO(type_parm)  bool ReadData(type_parm& val);

#define BinSerArithmeticDataI(type_parm, type_parm_id)                                                          \
BinSerData* BinSerData::GetInstance(type_parm& type_arithmetic_val){                                            \
  BinSerArithmeticData* ret_bsad_arithmetic_data = new BinSerArithmeticData(type_parm_id, type_arithmetic_val); \
  return ret_bsad_arithmetic_data;                                                                              \
}                                                                                                               

#define BinSerArithmeticDataO(type_parm, type_parm_id)                                                          \
bool BinSerData::ReadData(type_parm& type_arithmetic_val){                                                      \
  /*0. Check type */                                                                                            \
  if(type_id_ != type_parm_id) return false;                                                                    \
  /*1. Reinterpret type and get hte arithmeticdata */                                                           \
  BSAPTR(this)->GetArithmeticData(type_arithmetic_val);                                                         \
  return true;                                                                                                  \
}

#define BinSerContainerDataI(type_parm, type_parm_id, container_type_id)                                        \
BinSerData* BinSerData::GetInstance(type_parm& type_container_val){                                             \
  BinSerContainerData* container_data =                                                                         \
  new BinSerContainerData(type_parm_id, container_type_id, type_container_val);                                 \
  return container_data;                                                                                        \
}                                                                                                               

#define BinSerContainerDataO(type_parm, type_parm_id)                                                           \
bool BinSerData::ReadData(type_parm& type_container_val){                                                       \
  /*0. Check type */                                                                                            \
  if(type_id_ != type_parm_id) return false;                                                                    \
  /*1. Reinterpret type and get hte arithmeticdata */                                                           \
  BSCPTR(this)->GetContainerData(type_container_val);                                                           \
  return true;                                                                                                  \
}


/********************************************************************************/
/*                    User Defined Type Override Method MACRO                   */
/********************************************************************************/
#define UserDefinedSerDataParameter(Ser_para)               \
  bool WriteInData(BinSerUserDefinedTypeData* bsudt_data){  \
    BSUDTWriteExecutor bsudt_write_executor(bsudt_data);    \
    bsudt_write_executor+Ser_para;                          \
    return true;                                            \
  }                                                         \
  bool ReadOutData(BinSerUserDefinedTypeData* bsudt_data){  \
    BSUDTReadExecutor bsudt_read_executor(&bsudt_data);     \
    bsudt_read_executor+Ser_para;                           \
    return true;                                            \
  }                                                        

/********************************************************************************/
/*                           Deserialize method MACRO                           */
/********************************************************************************/
#define DESERIALIZEDATA(deserialize_val, buffer)                                                                \
    TypeId type_id = BinSerData::DeserializeTypeFromBuf(buffer);                                                \
    BinSerData* deserialize_val = nullptr;                                                                      \
    if(type_id>TypeId::invalid_t && type_id<TypeId::v_Ari_Con_bound){                                           \
      deserialize_val = BSAPTR(deserialize_val);                                                                \
      deserialize_val = new BinSerArithmeticData(type_id, 0);                                                   \
    }                                                                                                           \
    else if(type_id>TypeId::v_Ari_Con_bound && type_id<TypeId::v_Con_Urd_bound){                                \
      std::string empty_string_ = "";                                                                           \
      deserialize_val = BSCPTR(deserialize_val);                                                                \
      deserialize_val = new BinSerContainerData(type_id, ContainerTypeId::ct_invalid_t, empty_string_);         \
    }                                                                                                           \
    else if(type_id == TypeId::user_defined_t){                                                                 \
      deserialize_val = BSUDTPTR(deserialize_val);                                                              \
      deserialize_val = new BinSerUserDefinedTypeData(type_id);                                                 \
    }                                                                                                           \
    else {                                                                                                      \
      std::cout << "Sorry! Can not recgnize the file, Please ensure it !" << std::endl;                         \
      return false;                                                                                             \
    }                                                                                                           \
    if(!deserialize_val->DeserializeDataFromBuf(buffer)) return false;                      


/********************************************************************************/
/*                       Initial Capacity MACRO                                 */
/********************************************************************************/
const int DATA_BUFFER_INI_CAPACITY = 16; // The initial capacity of data buffer

const int BIN_SER_HEADER_LENGTH = 12; // The length of the header

const int BIN_SER_CONTAINER_HEADER_LENGTH = 9;

const int BIN_SER_BSUDT_HEADER_LENGTH = 8;

#endif