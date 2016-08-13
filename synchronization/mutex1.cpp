// "Copyright 2016 <Asit Dhal, dhal.asitk@gmail.com>"

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>

class Counter {
 public:
    Counter():value(0) { }

    void Increment() {
        std::lock_guard<std::mutex> guard(mutex);
        value++;
    }

    void Decrement() {
        std::lock_guard<std::mutex> guard(mutex);
        value--;
    }

    int getValue() {
        std::lock_guard<std::mutex> guard(mutex);
        return value;
    }

 private:
    std::mutex mutex;
    int value;
};

int main() {
    Counter counter;

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread([&counter]() {
            for (int i = 0; i < 100; ++i){
                counter.Increment();
            }
        }));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << counter.getValue() << std::endl;

    return 0;
}
