#ifndef CX_SEMAPHORE_H
#define CX_SEMAPHORE_H
#include <cstdint>
#include <condition_variable>
#include <mutex>

class CxSemaphore
{
public:
    CxSemaphore(uint32_t count = 0);
    ~CxSemaphore() = default;
    uint32_t Post();
    bool TryWait();
    uint32_t Wait();
private:
    std::condition_variable cv_;
    std::mutex m_;
    uint32_t count_{0};
};

#endif