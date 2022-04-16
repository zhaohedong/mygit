#ifndef SERVICE_H
#define SERVICE_H
#include <functional>
#include <queue>
#include <cstdint>
#include <thread>

class Service
{
public:
    Service() = default;
    ~Service() = default;
    uint32_t Init();
    uint32_t PushTask(std::function<void()> task);
    void Join();

private:
    void Run();
    std::queue<std::function<void()>> taskQueue_;
    std::unique_ptr<std::thread> thread_{nullptr};
};
#endif