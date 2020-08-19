#include <iostream>
#include "Derived.h"
 
using namespace std;
    Derived::Derived(){
        printf("Derived::Derived()\n");
    }
    Derived::~Derived(){
        printf("Derived::~Derived()\n");
    }
	void Derived::fun4(){
        printf("Derived::fun4\n");
    }
 
int main()
{
    Derived d;
    d.fun();
    d.fun2();
    d.fun3();
    d.fun4();
	return 0;
}
