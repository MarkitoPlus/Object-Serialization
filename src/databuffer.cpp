#include "databuffer.h"

using namespace mkbf;

BinaryDataBuffer::BinaryDataBuffer(): offset_(0), size_(0), capacity_(DATA_BUFFER_INI_CAPACITY), data_(nullptr){
  data_ = new char[capacity_];
}

void BinaryDataBuffer::WriteData(const char* data, const int len){
  /*Check the capacity of the data buffer*/
  while(!CheckCapacity(len)) Reallocate();   
  memcpy(data_+size_, data, len);
  size_ += len; 
}

bool BinaryDataBuffer::ReadData(char* data, const int len){
  if(offset_+len > size_) return false; //If overflow, return nullptr
  memcpy(data, data_+offset_, len);
  offset_ += len;
  return true;
}

bool BinaryDataBuffer::WriteDataToDataFile(std::fstream& fs){
  fs.write(data_, size_);
  return true;
}


bool BinaryDataBuffer::ReadDataFromDataFile(std::fstream& fs, const int len){
  while(!CheckCapacity(len)) Reallocate();  
  fs.read(data_+size_, len);
  size_ += len;
  return true;
}

int BinaryDataBuffer::GetSize(){
  return size_;
}

void BinaryDataBuffer::Clear(){
  delete[] data_;
  offset_ = 0;
  size_ = 0;
  capacity_ = DATA_BUFFER_INI_CAPACITY;
  data_ = new char[capacity_];
  return;
}

bool BinaryDataBuffer::CheckCapacity(int len){
  return size_+len <= capacity_? true: false;
}

void BinaryDataBuffer::Reallocate(){
  char* new_data_ = new char[capacity_*2];
  memcpy(new_data_, data_, size_); 
  delete[] data_;
  data_ = new_data_;
  capacity_ *= 2;
}
