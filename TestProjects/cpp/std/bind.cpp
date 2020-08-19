#include <functional>
#include <iostream>
 
class A
{
public:
    A()
    {
        k = 11;
        std::cout << "A constructor" << std::endl;
    }
 
    A(const A& rhs)
    {
        std::cout << "A copy constructor" << std::endl;
        k = rhs.k;
    }
 
    ~A()
    {
        std::cout << "A destructor" << std::endl;
    }
 
public:
    int k;
};
 
void f1(int i, int j, A a)
{
    std::cout << "k=" << a.k << ", i=" << i << ", j=" << j << std::endl;
}
 
int main()
{
 
   A* pa = new A();
   auto fx = std::bind(f1, 9, 10, *pa);
   pa->k = 100;
   delete pa;
 
   fx();
   return 0;
}