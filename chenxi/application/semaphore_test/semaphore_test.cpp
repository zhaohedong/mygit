#include <cstdint>
#include <thread>
// #include <iostream>
#include "cx_semaphore.h"

static CxSemaphore g_sem;
void waits()
{
    printf("Waiting... \n");
    g_sem.Wait();
    printf("Waiting finished\n");
}

void trywaits()
{
    printf("trywaits... \n");
    g_sem.TryWait();
    printf("trywaits finished\n");
}
 
void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Post ... \n");
    g_sem.Post();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Post ... \n");
    g_sem.Post();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Post ... \n");
    g_sem.Post();
    printf("Post finished... \n");
}

int main (int argc, char *argv[])
{
    std::thread t1(waits);
    std::thread t2(trywaits);
    std::thread t3(waits);
    std::thread t4(signals);
    t1.join(); 
    t2.join(); 
    t3.join(); 
    t4.join(); 
    return 0;
}