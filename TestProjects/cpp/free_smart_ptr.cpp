#include <iostream>
#include <string>
#include <memory>

class A {
public:
    A() 
    {
        std::cout << "A::A()" << std::endl;
    }
    ~A() 
    {
        std::cout << "A::~A()" << std::endl;
    }
    int fun() 
    {
        std::cout << "hello fun" << std::endl;
        return 0;
    }
};

class B {
public:
    std::unique_ptr<A> pA;
    B() {std::cout << "B::B()" << std::endl;}
    ~B() 
    {
        if (pA)
        {
            std::cout << "pA not nullptr!" << std::endl;
            pA = nullptr; // Will call A::~A()
        }
        std::cout << "B::~B()" << std::endl;
    }
};

int main()
{
    std::cout << "1" << std::endl;
    std::unique_ptr<A> pA = std::make_unique<A>();
    pA = nullptr;
    std::cout << "2" << std::endl;
}