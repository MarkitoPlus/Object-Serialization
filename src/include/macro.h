#ifndef MACRO_H
#define MACRO_H

/********************************************************************************/
/*                                 Debug MACRO                                  */
/********************************************************************************/
#define ASSERT(expr, message) assert((expr) && (message))


/********************************************************************************/
/*                              Magic Number MACRO                              */
/********************************************************************************/
const int BSEH_MAGIC_NUM = 838981;

const int BSCH_MAGIC_NUM = 838982;

const int BSUDT_MAGIC_NUM = 838983;


/********************************************************************************/
/*                    User Defined Type Override Method MACRO                   */
/********************************************************************************/
#define UserDefinedSerDataParameter(Ser_para)               \
  bool WriteInData(BinSerUserDefinedTypeData& bsudt_data){  \
    BSUDTWriteExecutor bsudt_write_executor(&bsudt_data);   \
    bsudt_write_executor+Ser_para;                          \
    return true;                                            \
  }                                                         \
  bool ReadOutData(BinSerUserDefinedTypeData& bsudt_data){  \
    BSUDTReadExecutor bsudt_read_executor(&bsudt_data);     \
    bsudt_read_executor+Ser_para;                           \
    return true;                                            \
  }                                                        


/********************************************************************************/
/*                      Data Buffer Initial Capacity MACRO                      */
/********************************************************************************/
const int DATA_BUFFER_INI_CAPACITY = 16;

#endif