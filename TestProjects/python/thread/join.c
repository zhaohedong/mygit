#include <iostream>
#include <thread>
#include <chrono>
 
void foo()
{
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "waiting for helpers to finish11..." << std::endl;
}
 
void bar()
{
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "waiting for helpers to finish22..." << std::endl;
}
 
int main()
{
    std::cout << "starting first helper...\n";
    std::thread helper1(foo);
 
    std::cout << "starting second helper...\n";
    std::thread helper2(bar);
 
    std::cout << "waiting for helpers to finish..." << std::endl;
    helper1.join();
    std::cout << "waiting for helpers to finish1..." << std::endl;
    helper2.join();
    std::cout << "waiting for helpers to finish2..." << std::endl;
 
    std::cout << "done!\n";
}
