#include <iostream>

#define BLUETOOTH_DELETE_COPY_AND_COPY_ASSIGN(CLASSNAME) \
CLASSNAME(const CLASSNAME&) = delete; \
CLASSNAME& operator=(const CLASSNAME&) = delete; 

#define BLUETOOTH_DELETE_MOVE_AND_MOVE_ASSIGN(CLASSNAME) \
CLASSNAME(CLASSNAME&&) = delete; \
CLASSNAME& operator=(CLASSNAME&&) = delete;

#define BLUETOOTH_DELETE_COPY_AND_MOVE(CLASSNAME) \
CLASSNAME(const CLASSNAME&) = delete; \
CLASSNAME& operator=(const CLASSNAME&) = delete; \
CLASSNAME(CLASSNAME&&) = delete; \
CLASSNAME& operator=(CLASSNAME&&) = delete;

#define BLUETOOTH_DEFAULT_COPY_AND_MOVE(CLASSNAME) \
CLASSNAME(const CLASSNAME&) = default; \
CLASSNAME& operator=(const CLASSNAME&) = default; \
CLASSNAME(CLASSNAME&&) = default; \
CLASSNAME& operator=(CLASSNAME&&) = default;

class Person {
public:
    static int counter;
    Person() {
        counter++;
        std::cout << "构造函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
#if 0
    Person(const Person& pr) {
        counter++;
        std::cout << "拷贝构造函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
    Person& operator=(const Person& pr) {
        std::cout << "赋值运算函数" << std::endl;
        return *this;
    }
#endif
    virtual ~Person() {
        counter--;
        std::cout << "析构函数" << std::endl;
        std::cout << "counter:" << counter << std::endl;
    }
private:
    BLUETOOTH_DELETE_COPY_AND_MOVE(Person)
};

//~ Person.cpp
//#include "Person.h"

int Person::counter = 0;

int main()
{
 Person p1;
 Person p2;
 //p2 = p1;
 //Person p3 = p1;
 //Person p4 = Person(p1);
 Person *p5 = &p1;
 printf("int is %ld\n",sizeof(int));
 printf("unsigned long is %ld\n",sizeof(unsigned long));
 int *p = 0;
 printf("point is %ld\n",sizeof(p));
 return 0;
}