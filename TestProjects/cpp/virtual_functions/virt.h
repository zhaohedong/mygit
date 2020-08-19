#include <iostream>
 
using namespace std;

class Orginal 
{
public:
    virtual ~Orginal(){}
    virtual void fun(){} 
    virtual void fun2()=0;
};

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

class Derived : public Base
{
    public:
            Derived();
            ~Derived();
            void fun4();
};
 
