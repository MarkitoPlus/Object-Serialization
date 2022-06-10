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
}

bool BinSerData::DeserializeDataFromBuf(mkbf::BinaryDataBuffer* binary_data_buffer){
  ASSERT(false, "Call a pure function! ");
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
     val = arithmetic_data_.char_val_l;
    break;
  case TypeId::int_t:
    val = arithmetic_data_.int_val_ ;
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
explicit BinSerContainerDataHeader::BinSerContainerDataHeader(int size, ContainerTypeId container_type_id)\
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
template<typename T>
void BinSerContainerData::WriteSingleType(const T& val){
  int i = 0;
  for(auto it = val.begin(); it != val.end(); it++) 
    bin_ser_container_data_.at(i++)->GetInstance(*it);
  return;
}

template<typename T>
void BinSerContainerData::WriteDoubleType(const T& val){
  int i = 0;
  for(auto it = val.begin(); it != val.end(); it++){
    bin_ser_container_data_.at(i++)->GetInstance(it->first);
    bin_ser_container_data_.at(i++)->GetInstance(it->second);
  }
  return;
}

template<typename T>
BinSerContainerData::BinSerContainerData(TypeId type_id, ContainerTypeId container_type_id, const T& val): \
  BinSerData(type_id), bin_ser_container_data_header_(val.size(), container_type_id){
  switch(bin_ser_container_data_header_->container_type_id_)
  {
  case ContainerTypeId::ct_single_t:
    WriteSingleType(val);
    break;
  case ContainerTypeId::ct_double_t:
    WriteDobleType(val);
    break;
  default:
    ASSERT(false, "Not a valid container type");
  }
  return;
}

template<typename T>
void BinSerContainerData::ReadSingleType(T& val){
  /*note: more details in Devlog.md 6/9 */
  /*0. Record the current size of the container that user offers and resize it */
  int pre_num = val.size();
  val.resize(pre_num + bin_ser_container_data_header_->size_);

  /*1. move the iterator to the correct position */
  int i = 0; 
  auto c_it = val.begin();
  for(; i<pre_num; c_it++, i++);

  /*2. Copy the actual data */
  for(i = 0; c_it != val.end(); c_it++){
    bin_ser_container_data_.at(i++)->ReadData(*c_it);
  }

  return;
}

template<typename T>
void BinSerContainerData::ReadDoubleType(T& val){
  /*0. Record the current size of the container that user offers and resize it */
  int pre_num = val.size();
  val.resize(pre_num + bin_ser_container_data_header_->size_);

  /*1. move the iterator to the correct position */
  int i = 0;
  auto c_it = val.begin();
  for(; i<pre_num; c_it++, i++);

  /*2. Copy the actual data */
  for(i = 0; c_it != val.end(); c_it++){
    bin_ser_container_data_.at(i++)->ReadData(c_it->first);
    bin_ser_container_data_.at(i++)->ReadData(c_it->second);
  }

  return;
}

template<typename T>
bool BinSerContainerData::GetContainerData(T& val){
  switch(bin_ser_container_data_header_->container_type_id_)
  {
  case ContainerTypeId::ct_single_t:
    ReadSingleType(val);
    break;
  case ContainerTypeId::ct_double_t:
    ReadDoubleType(val);
    break;
  default:
    ASSERT(false, "Not a valid container type");
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
  int size = (bin_ser_container_data_header_->container_type_id_ == ContainerTypeId::ct_single_t)? \
              bin_ser_container_data_header_->size_: 2*bin_ser_container_data_header_->size_;
  for(int i = 0; i<size; i++){
    DESERIALIZEDATA(deser_data, binaray_data_buffer)
    bin_ser_container_data_.emplace_back(deser_data);
  }
}


/********************************************************************************/
/*                          BinSerUserDefinedTypeDataHeader                     */
/********************************************************************************/
explicit BinSerUserDefinedTypeDataHeader::BinSerUserDefinedTypeDataHeader(int size): size_(size){
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
explicit BinSerUserDefinedTypeData::BinSerUserDefinedTypeData(TypeId type_id): BinSerData(type_id), bsudt_data_header_(0), offset_(0){
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
explicit BSUDTWriteExecutor::BSUDTWriteExecutor(BinSerUserDefinedTypeData* bsudt): bsudt_(bsudt){
}

template<typename T>
BSUDTWriteExecutor& BSUDTWriteExecutor::operator+(const T& val){
  bsudt_>bsudt_data_.emplace_back(BinSerData::GetInstance(val));
  return *this;
}

explicit BSUDTReadExecutor::BSUDTReadExecutor(BinSerUserDefinedTypeData* bsudt): bsudt_(bsudt){
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
explicit BinSerExecutorHeader::BinSerExecutorHeader(): size_(0), n_byte_(0), offset_(0){
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
explicit BinSerExecutor::BinSerExecutor(){
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

template<typename T>
bool BinSerExecutor::Enqueue(const T val){
  binary_serialization_data_.emplace_back(BinSerData::GetInstance(val));
  return true;
}

template<typename T>
bool BinSerExecutor::Dequeue(T& val){
  if(binary_serialization_executor_header_->offset_ >= binary_serialization_data_.size()) return false;
  else if(!binary_serialization_data_.at(binary_serialization_executor_header_->offset_)->ReadData(val)) return false;
  binary_serialization_executor_header_->offset_++;
  return true;
}


