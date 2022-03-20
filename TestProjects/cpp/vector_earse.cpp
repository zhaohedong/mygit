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

typedef struct s_
{
    int a{0};
}s;

class B
{
 public:
  B(A& a1) : a(a1){}
   ~B(){}
   A& a;
   int b{0};
};

int main ()
{
    std::vector<A*> arrayA;
    A* a1 = new A();
    A* a2 = new A();
    A* a3 = new A();
    arrayA.push_back(a1);
    arrayA.push_back(a2);
    arrayA.push_back(a3);



  return 0;
}
