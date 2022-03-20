#include <iostream>
// class B {
// public:
//     B() {
//         std::cout << "B()" << std::endl;
//     }
//     virtual ~B() {
//         std::cout << "~B()" << std::endl;
//     }

//     void funcB(int b) {
//         // b_ = b;
//     }

//     int b_{0};
// };

// class A {
// public:
//     A() {
//         std::cout << "A()" << std::endl;
//     }
//     virtual ~A() {
//         std::cout << "~A()" << std::endl;
//     }

//     void funcA(int a) const {
//         objB_->funcB(a);
//     }

//     B* objB_;
// };


void  fun(const int* const p) {
    int a = *p;
    std::cout << a << std::endl;
}


int main()
{
    int c = 9;
    int *b = new int;
    const int* p = &c;
    int *p1 = const_cast<int*>(p);
    fun(&c);
    fun(b);
    return 0;
}


