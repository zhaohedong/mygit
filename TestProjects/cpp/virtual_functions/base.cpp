#include <iostream>
#include "virt.h"
 
using namespace std;
    Base::Base(){
        printf("Base::Base()\n");
    }
    Base::~Base(){
        printf("Base::~Base()\n");
    }
	void Base::fun() {
        printf("base::fun\n");
    };
	void Base::fun2(){
        printf("base::fun2\n");
    }
	void Base::fun3(){
        printf("base::fun3\n");
    }
#if 0
	void Base::fun4(){
        printf("base::fun4\n");
    }
#endif


