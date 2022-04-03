#include "service_test.h"
#include <cstdint>
// #include <iostream>
#include "service.h"

static int func(int a, int b)
{
    auto c = a + b;
    printf("a+b = %d\n",c);
    return 1;
}

int main (int argc, char *argv[])
{
    Service service;
    service.Init();
    auto f = []() -> int {
        // std::cout << "lambda function from main" << std::endl;
        printf("lambda function from main 1\n");
        return 0;
    };

    auto f2 = []() -> int {
        // std::cout << "lambda function from main" << std::endl;
        printf("lambda function from main 2\n");
        return 0;
    };

    auto localFunc = std::bind(&func, 1, 3);
    service.PushTask(f);
    service.PushTask(f2);
    service.PushTask(localFunc);
    service.Join();
    return 0;
}