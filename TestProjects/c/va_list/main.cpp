#include <iostream>
#include <cstdarg>
#include <cmath>

class A
{
public:
    A(){
        std::cout << "A() a_ = " << a_ << std::endl;
    }
    int a_ = 3;
};

class B
{
public:
    B(int b):b_(b){
        std::cout << "B() b_ = " << b_ << std::endl;

    }
    int b_ = 4;
};

class C
{
public:
    C(int c, int d):c_(c), d_(d){

        std::cout << "C() c_ = " << c_ << std::endl;
        std::cout << "C() d_ = " << d_ << std::endl;
    }
    int c_ = 5;
    int d_ = 5;
};

#define SCREEN1_ID  0
#define SCREEN2_ID  1
#define SCREEN3_ID  2

double stddev(int count, ...) 
{
    double sum = 0;
    double sum_sq = 0;
    std::va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        double num = va_arg(args, double);
        sum += num;
        sum_sq += num*num;
    }
    va_end(args);
    return std::sqrt(sum_sq/count - (sum/count)*(sum/count));
}

template < typename T >
void* CreateObject(int count, ...)
{
    std::va_list args;
    va_start(args, count);
    void *p = nullptr;
    int buf[64] = {};
    for (int i = 0; i < count; ++i) {
        buf[i] = va_arg(args, int);
    }
    va_end(args);

    switch(count)
    {
 #if 1
            case SCREEN1_ID:
                    p = new T();
                    break;
            case SCREEN2_ID:
                    p = new T(buf[0]);
                    break;
 #endif
            case SCREEN3_ID:
                    p = new T(buf[0], buf[1]);
                    break;
            default:
                    break;
    
    }
} 
int main() 
{
 //   CreateObject<A>(0);
//    CreateObject<B>(1,124);
    CreateObject<C>(2,124,256);
    return 0;
}
