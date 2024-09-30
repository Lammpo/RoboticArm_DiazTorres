#include "AsyncTask.h"

AsyncTask::AsyncTask(unsigned long initialDelay,
                     std::function<bool()> checkFunc,
                     std::function<void()> tickFunc,
                     std::function<unsigned long()> recalculoDelayFunc)
    : antes(micros()),
      microDelay(initialDelay),
      check(checkFunc),
      tick(tickFunc),
      recalculoDelay(recalculoDelayFunc) {}

void AsyncTask::run() {
    if (micros() - antes > microDelay && check()) {
        antes = micros();
        tick();
        microDelay = recalculoDelay();
    }
}
