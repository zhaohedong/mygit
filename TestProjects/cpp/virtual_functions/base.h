#ifndef BASE_H
#define BASE_H
#include <iostream>
#include "Orginal.h"
 
using namespace std;

class Base : public Orginal
{
public:
    Base();
    ~Base();
	virtual void fun();
	virtual void fun2();
	virtual void fun3();

#if 1 
	virtual void fun4();
#else
	virtual void fun4(){
        printf("base::fun4\n");
    }
#endif
};

#endif
