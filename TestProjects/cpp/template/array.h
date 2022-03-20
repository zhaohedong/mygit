#ifndef ARRAY_H
#define ARRAY_H
template <typename T, int SIZE>
class array
{
      T data_[SIZE];
      array (const array& other);
      const array& operator = (const array& other);
  public:
      array(){};
      T& operator[](int i);
      const T& get_elem (int i) const;
      void set_elem(int i, const T& value);
      operator T*();      
};        
#include "array.cpp"
#endif