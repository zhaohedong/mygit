#include "array.h"

int main(void)
{
array<int, 50> intArray;
intArray.set_elem(0, 2);
int firstElem = intArray.get_elem(0);
int* begin = intArray;
}