#include "service.h"
#include <thread>

uint32_t Service::Init()
{
    thread_ = std::make_unique<std::thread>(&Service::Run, this); 
    return 0;
}

uint32_t Service::PushTask(std::function<void()> task)
{
    taskQueue_.push(task);
    return 0;
}

void Service::Join()
{
    thread_->join();
}

void Service::Run()
{
    while (1) {
        if (!taskQueue_.empty()) {
            auto task = taskQueue_.front();
            taskQueue_.pop();
            task();
            printf("Helo Service!\n");
        }
    }
}