#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include <Arduino.h>
#include <functional>

class AsyncTask {
private:
    unsigned long antes;
    unsigned long microDelay;
    std::function<bool()> check;
    std::function<void()> tick;
    std::function<unsigned long()> recalculoDelay;

public:
    AsyncTask(unsigned long initialDelay,
              std::function<bool()> checkFunc,
              std::function<void()> tickFunc,
              std::function<unsigned long()> recalculoDelayFunc);

    void run();
};

#endif // ASYNCTASK_H
