// map::find
#include <iostream>
#include <vector>

struct A
{
  int a;
  int b;
  int c;
};

std::vector<A> myVecs;


std::vector<A>::iterator GetA(int a)
{
  std::vector<A>::iterator it;
  for (it = myVecs.begin(); it != myVecs.end(); it++){
    if (it->a == a) {
      std::cout << "elements matched: it->a is " << it->a << " ,a is " << a << '\n';
      break;
    }
  }
  return it;
}

int main ()
{
  myVecs.push_back({1,1,1});
  myVecs.push_back({2,2,2});
  myVecs.push_back({3,3,3});

  for (auto ita = myVecs.begin(); ita != myVecs.end(); ita++){
      std::cout << "ita->a is " << ita->a << ", ita->b is " << ita->b << ", ita->c is " << ita->c << '\n';
  }

  auto it = GetA(2);
  it->b = 3;
  it->c = 4;

  for (auto ita = myVecs.begin(); ita != myVecs.end(); ita++){
      std::cout << "ita->a is " << ita->a << ", ita->b is " << ita->b << ", ita->c is " << ita->c << '\n';
  }

  it = GetA(4);
  if (it == myVecs.end()) {
    std::cout << "elements match failed!!" << '\n';
  }
  return 0;
}