#ifndef DERIVED_H
#define DERIVED_H
#include <iostream>
#include "base.h"
 
using namespace std;

class Derived : public Base
{
    public:
            Derived();
            ~Derived();
            void fun4();
};
#endif 
