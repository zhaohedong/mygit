// map::find
#include <iostream>
#include <map>
#include <vector>

struct A {
    char a;
    int b;
};

int main ()
{
    A a1{'a',50};
    A a2{'b',100};
    A a3{'c',150};
    A a4{'d',200};

    std::vector<A> mymap;
    mymap.push_back(a1);
    mymap.push_back(a2);
    mymap.push_back(a3);
    mymap.push_back(a4);

    int to_find = 200;
    bool isExist = true;

    auto it = std::find_if(mymap.begin(), mymap.end(), [&to_find](const A &a) { return to_find == a.b;});

    if (it != mymap.end()) {
        isExist = true;
    }

    std::cout << "it->a is " << it->a << "," "<< it->b " << it->b << std::endl;

  return 0;
}