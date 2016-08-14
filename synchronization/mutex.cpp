#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>

class Counter {
 public:
    Counter():value(0) { }

    void increment(int v) {
        // std::lock_guard provides RAII idiom for mutex, it locks the mutex
        // on construction and unlocks on destruction.
        std::lock_guard<std::mutex> guard(mutex);
        value += v;
    }

    void decrement(int v) {
        std::lock_guard<std::mutex> guard(mutex);
        value -= v;
    }

    int getValue() {
        std::lock_guard<std::mutex> guard(mutex);
        return value;
    }

 private:
    // c++11 synchronization primitive to assure mutual exclusion
    // in critical section.
    std::mutex mutex;
    int value;
};

int main() {
    Counter counter;

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread([&counter]() {
            for (int i = 0; i < 100; ++i){
                if (i%2 == 1)
                    counter.increment(i);
                else
                    counter.decrement(i);
            }
        }));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << counter.getValue() << std::endl;

    return 0;
}
