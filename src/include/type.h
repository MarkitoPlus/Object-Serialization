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
  string_t,
  std_pair_t,
  std_vector_t,
  std_list_t,
  std_set_t,
  std_map_t,
  user_defined_t
};

#endif