#include <iostream>
#include <vector>
class A
{
public:
  A (int x_arg) : x (x_arg) { std::cout << "A (x_arg)\n"; }
  A () { x = 0; std::cout << "A ()\n"; }
  A (const A &rhs) noexcept { x = rhs.x; std::cout << "A (A &)\n"; }
  A (A &&rhs) noexcept { x = rhs.x; std::cout << "A (A &&)\n"; }
  A& operator=(const A& other){ this->x = other.x; std::cout << "A& operator=(const A& other)\n"; return *this; }

private:
  int x;
};

int main ()
{
    A a1(1);
    for (int i = 0; i < 5; i++) {
        A a2 = a1;
        std::cout << "a2 address is " << &a2 << std::endl; 
    }

    std::cout << "outside loop" << std::endl;    
    A a3(2);
    for (int i = 0; i < 5; i++) {
        a3 = a1;
        std::cout << "a2 address is " << &a3 << std::endl; 
    }

  return 0;
}