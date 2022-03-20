#include <iostream>
#include <map>

class A {
public:
    void funA()
    {
        printf("a = %d", a);
    }
private:
    int a{3};
};

int main ()
{
  std::map<char,A> mymap;
#if 0


  for (c='a'; c<'h'; c++)
  {
    std::cout << c;
    if (mymap.count(c)>0)
      std::cout << " is an element of mymap.\n";
    else 
      std::cout << " is not an element of mymap.\n";
  }
  mymap ['x'] = 200;
#endif
  mymap['x'].funA();

  return 0;
}