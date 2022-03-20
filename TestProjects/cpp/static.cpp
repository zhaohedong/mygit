

//~ Person.h

#ifndef PERSON_H_
#define PERSON_H_
#include <iostream>

class Person {
private:
    static int counter;
    
public:
    static Person& GetInstance() {
        static Person instance;
        return instance;
    }
    static std::string sStr;
    Person() {
        counter++;
        std::cout << "构造函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
    Person(const Person& pr) {
        counter++;
        std::cout << "拷贝构造函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
    Person& operator=(const Person& pr) {
        std::cout << "赋值运算函数" << std::endl;
        return *this;
    }
    virtual ~Person() {
        counter--;
        std::cout << "析构函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
};

std::string Person::sStr;

#endif /* PERSON_H_ */

//~ Person.cpp
//#include "Person.h"

int Person::counter = 0;

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
    }
    virtual ~A() {
        std::cout << "~A()" << std::endl;
    }
    private:
    Person& person{Person::GetInstance()};
};

void fun() {
    std::cout << "entry fun" << std::endl;
     A *p = new A();
 std::cout << "new A" << std::endl;
 delete p;
 std::cout << "delete p" << std::endl;
    std::cout << "exit fun" << std::endl;
}


int main()
{
    fun();
 return 0;
}


