#include "cx_semaphore.h"
#include <thread>
CxSemaphore::CxSemaphore(uint32_t count) : count_(count)
{
}

uint32_t CxSemaphore::Post()
{
    printf("%s start acquire lock\n", __FUNCTION__);
    std::lock_guard lk(m_);
    printf("%s end acquire lock\n", __FUNCTION__);
    printf("%s start\n", __FUNCTION__);
    count_++;
    cv_.notify_one();
    printf("%s count_[%d]\n", __FUNCTION__, count_);
    printf("%s end\n", __FUNCTION__);
    return 0;
}

bool CxSemaphore::TryWait()
{
    std::lock_guard lk(m_);
    printf("%s start\n", __FUNCTION__);
    if (count_ <= 0)
    {
        printf("Try wait failed count_ = %d\n", count_);
        return 1;
    }
    else
    {
        printf("Try wait passed count_ = %d\n", count_);
        return 0;
    }
    printf("%s count_[%d]\n", __FUNCTION__, count_);
    printf("%s end\n", __FUNCTION__);
    return 0;
}

uint32_t CxSemaphore::Wait()
{
    std::unique_lock lk(m_);
    printf("%s start acquire lock\n", __FUNCTION__);
    cv_.wait(lk, [&]{ 
                printf("%s end acquire lock\n", __FUNCTION__);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                printf("%s release acquire lock\n", __FUNCTION__);
                return (count_ > 0);
            });
    count_--;
    printf("%s count_[%d]\n", __FUNCTION__, count_);
    printf("%s end\n", __FUNCTION__);
    return 0;
}