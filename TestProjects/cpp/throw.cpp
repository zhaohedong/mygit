#include<iostream>                                 //包含头文件
#include<stdlib.h>
using namespace std;
#include <exception>
class A
{
public:
    A(int a){
    // try
    // {
        if (a == 7) throw "a == 7";
        a_ = a;
        

    // }
    // catch(...)
    // {
    //     // 能处理任何异常的代码
    //     cout << "catch a == 7: " << endl;
    //     //exit(1);
    // };

    }
    int a_ = 0;

};

class B
{
public:
    B(int b){
    // try
    // {
        if (b == 7) throw std::out_of_range;
        b_ = b;
        

    // }
    // catch(...)
    // {
    //     // 能处理任何异常的代码
    //     cout << "catch a == 7: " << endl;
    //     //exit(1);
    // };

    }
    int b_ = 0;

};

void funA(const A& a){
    cout <<"funA A.a_ :" << a.a_ << endl;  
}

double fuc(double x, double y)                        //定义函数
{
    if(y==0)
    {
        throw y;                                    //除数为0，抛出异常
    }
    return x/y;                                    //否则返回两个数的商
 }
int main()
{
    //  double res;
    //  try                                            //定义异常
    //  {
    //      res=fuc(2,3);
    //      cout<<"The result of x/y is : "<<res<<endl;
    //      res=fuc(4,0);                                //出现异常
    //  }
    //  catch(double)                                    //捕获并处理异常
    //  {
    //      cerr<<"error of dividing zero.\n";
    //      exit(1);                                    //异常退出程序
    //  }
    A* pA = (A*)new B(5);

    try {
        pA = new A(7);
    } catch(...) {
        cout <<"catch pA: " << pA << endl; 
    }
    funA(*pA);
    return 0;
 }
