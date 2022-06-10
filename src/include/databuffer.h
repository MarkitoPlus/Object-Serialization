#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <fstream>
#include "macro.h"

namespace mkbf{

class BinaryDataBuffer{
public:
  /**
   * @brief Create the binary data buffer and initialize member value
   * 
   * @note Binary Data Buffer short for buffer below
   */
  explicit BinaryDataBuffer();
  
  /**
   * @brief Write Data with specific length into  buffer
   * 
   * @param data the source data 
   * 
   * @param len the length of the data, unit: char
   * 
   * @note Shallow copy! It means after the data be copied into buffer, it won't change any more
   * 
   * @note Check capacity before writing data. If necessary, reallocate it. 
   * 
   * @note the offset of the buffer will be adjusted during writing.
   */
  void WriteData(const char* data, const int len);

  /**
   * @brief Read out data from buffer 
   * 
   * @param len the length of the data to be read, unit: char
   * 
   * @return char* 
   * 
   * @note the offset of the buffer will be adjusted during writing.
   */
  bool ReadData(char* data, const int len);

  /**
   * @brief Write the whole data into .data file
   * 
   * @param fs the .data file 
   */
  bool WriteDataToDataFile(std::fstream& fs);

  /**
   * @brief Read out data from the .data file
   * 
   * @param fs the .data file
   */
  bool ReadDataFromDataFile(std::fstream& fs, const int len);

  /**
   * @brief Helper method to get the total bytes stored in buffer
   * 
   * @return the total bytes
   */
  int GetSize();

  /**
   * @brief Clear all the data in buffer and reset its status
   * 
   */
  void Clear();
private:
  
  /**
   * @brief Check the capacity of the buffer before writing data
   * 
   * @param len the length of writing data
   * 
   * @return true: valid
   * 
   * @return false: invalid 
   */
  bool CheckCapacity(int len);

  /**
   * @brief Reallocate the buffer, expand it
   * 
   */
  void Reallocate();
  
private:
  int   offset_;

  int   size_;

  int   capacity_;

  char* data_;
};

}

#endif