#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class semaphore {
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0;

public:
    void release();
    void acquire();
    bool try_acquire();
};

#endif /* _SEMAPHORE_H_ */