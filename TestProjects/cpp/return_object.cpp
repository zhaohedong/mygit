#include <iostream>
using namespace std;
class Test{
        public:
    Test() {cout<<"Constructor"<<endl;}
    Test(const Test &){cout<<"copy Constructor"<<endl;}
    ~Test() {cout<<"destroy"<<endl;}
};
class TestA{
        public:
    TestA() {cout<<"TestA Constructor"<<endl;}
    TestA(const TestA &){cout<<"copy TestA Constructor"<<endl;}
    ~TestA() {cout<<"destroy TestA"<<endl;}
};
Test fun()
{
    Test t1;
    cout<<"&t1 is "<<&t1<<endl;
    return t1;
}
TestA& funA()
{
    TestA tA;
    cout<<"&tA is "<<&tA<<endl;
    return tA;
}
int main()
{
    Test t2 = fun();
    cout<<"&t2 is "<<&t2<<endl;
    cout<<"This is a test!"<<endl;
    TestA& tA = funA();
    cout<<"&tA is "<<&tA<<endl;
    return 0;
}
