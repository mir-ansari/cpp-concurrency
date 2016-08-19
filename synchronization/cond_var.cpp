#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

// mutex to protect condition variable
std::mutex workMutex;
// condition variable
std::condition_variable condVar;

int inputData, outputData;

// condition variable predicates
bool ready, processed;

void calculateFactorial() {
    // lock the mutex with unique_lock
    std::unique_lock<std::mutex> lk(workMutex);
    // wait on condition varibel
    // now the mutex will unlocked and the thread will be blocked
    // once the predicate is true, the thread will be unbloked
    // after that mutex will be locked
    // predicate is a lambda function, mostly checks a condition
    condVar.wait(lk, []{ return ready;});

    std::cout << "Worker working on data.." << std::endl;
    outputData = 1;
    if (inputData < 1) {
        outputData = 1;
    } else {
        for (int i=1; i <= inputData; i++) {
            outputData *= i;
        }
    }
    processed = true;
    // mutex is unlocked
    lk.unlock();
    condVar.notify_one();
}

int main(int argc, char* argv[]) {
    std::thread t1(calculateFactorial);

    outputData = 1;
    inputData = 5;
    {
        // lock the mutex and change the perdicate
        std::lock_guard<std::mutex> lk(workMutex);
        ready = true;
    }
    // inform the waiting thread
    condVar.notify_one();

    {
        std::cout << "Worker given to work" << std::endl;
        std::unique_lock<std::mutex> lk(workMutex);
        condVar.wait(lk, []{return processed;});
    }

    std::cout << "Factorial of " << inputData << ": "
        << outputData << std::endl;

    outputData = 1;
    inputData = 23;
    {
        std::lock_guard<std::mutex> lk(workMutex);
        ready = true;
    }
    condVar.notify_one();
    {
        std::cout << "Worker given to work" << std::endl;
        std::unique_lock<std::mutex> lk(workMutex);
        condVar.wait(lk, []{return processed;});
    }
    std::cout << "Factorial of " << inputData << ": "
        << outputData << std::endl;

    t1.join();

    return 0;
}
