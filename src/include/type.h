#ifndef TYPE_H
#define TYPE_H
/*
 * Define the enum class with the unique id for each valid typeid in
 * both the binaryserialization module and xmlserialization module
 */
enum class TypeId: unsigned char{
  invalid_t,
  bool_t,
  char_t,
  int_t,
  float_t,
  double_t,
  v_Ari_Con_bound,
  string_t,
  std_pair_t,
  std_vector_t,
  std_list_t,
  std_set_t,
  std_map_t,
  v_Con_Urd_bound,
  user_defined_t
};

/*
 * Arithmetic data type
 */
union U_Arithmetic_Data{
  bool    bool_val_;
  char    char_val_;
  int     int_val_;
  float   float_val_;
  double  double_val_;
};

/*
 * Define the enum class with the unique id for container
 */
enum class ContainerTypeId: unsigned char{
  ct_invalid_t,
  ct_single_t, // The container only stores single type data eg. std::vector, std::list
  ct_double_t  // The container that stores two kinds of data type eg. std::map, std::pair
};

#endif