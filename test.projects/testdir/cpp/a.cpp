//~ Person.h

#ifndef PERSON_H_
#define PERSON_H_
#include <iostream>
class Person {
private:
    static int counter;
public:
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

#endif /* PERSON_H_ */

//~ Person.cpp
//#include "Person.h"

int Person::counter = 0;

int main()
{
 Person p1;
 Person p2;
 p2 = p1;
 Person p3 = p1;
 Person p4 = Person(p1);
 Person *p5 = &p1;
 printf("int is %ld\n",sizeof(int));
 printf("unsigned long is %ld\n",sizeof(unsigned long));
 int *p = 0;
 printf("point is %ld\n",sizeof(p));
 return 0;
}
