#include <iostream>
#include "virt.h"
 
using namespace std;
#if 0
    Base::Base(){
        printf("Base::Base()\n");
    }
    Base::~Base(){
        printf("Base::~Base()\n");
    }
#endif
	void Base::fun() {
        printf("base::fun\n");
    };
	void Base::fun2(){
        printf("base::fun2\n");
    }
	void Base::fun3(){
        printf("base::fun3\n");
    }
#if 1 
	void Base::fun4(){
        printf("base::fun4\n");
    }
#endif
 
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
