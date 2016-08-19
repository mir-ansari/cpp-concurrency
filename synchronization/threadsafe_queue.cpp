#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

template<typename T>
class ThreadsafeQueue {
 public:
    ThreadsafeQueue() {}

    ThreadsafeQueue(ThreadsafeQueue const& other) {
        std::lock_guard<std::mutex> lock(other.m_mut);
        m_dataQueue = other.m_dataQueue;
    }

    // assignment is not allowed
    ThreadsafeQueue& operator=( const ThreadsafeQueue&) = delete;

    void push(T newValue) {
        std::lock_guard<std::mutex> lock(m_mut);
        m_dataQueue.push(newValue);
        m_dataCond.notify_one();
    }

    // the calling thread will be bloked if waitAndPop is called
    // when queue is empty
    void waitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(m_mut);
        m_dataCond.wait(lock, [this]{return !m_dataQueue.empty();});
        value = m_dataQueue.front();
        m_dataQueue.pop();
    }

    // it will return the data wrapped in a shared_ptr
    std::shared_ptr<T> waitAndPop() {
        std::unique_lock<std::mutex> lock(m_mut);
        m_dataCond.wait(lock, [this]{return !m_dataQueue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
        return res;
    }

    // calling thread will return immediately
    bool tryPop(T& value) {
        std::lock_guard<std::mutex> lock(m_mut);
        if (m_dataQueue.empty())
            return false;
        value = m_dataQueue.front();
        m_dataQueue.pop();
        return true;
    }

    std::shared_ptr<T> tryPop() {
        std::lock_guard<std::mutex> lock(m_mut);
        if (m_dataQueue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mut);
        return m_dataQueue.empty();
    }

 private:
    // mutex should be allowed to be modified in a constant
    // function, e.g. empty()
    mutable std::mutex m_mut;

    // stl queue, not thread safe
    std::queue<T> m_dataQueue;

    // condition varibela will wake up any bloked thread
    std::condition_variable m_dataCond;
};

ThreadsafeQueue<int> tsQueue;


// producer which pushes data into the queue
void runProducer(int total) {
    for (int i = 0; i < total; i++) {
        tsQueue.push(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// consumer which gets data from the queue
void runConsumer() {
    int k;
    while (true) {
        tsQueue.waitAndPop(k);
        std::cout << "Got an int: " << k << std::endl;
    }
}

// in the main function, there is one consumer and 2 producers who
// concurrently accesses the shared queue. The producers and consumers
// don't need any synchronization.
int main() {
    std::vector<std::thread> threads;
    for (int i=0; i < 3; i++) {
        threads.push_back(std::thread(runProducer, i*10));
    }

    std::thread t1(runConsumer);

    if (t1.joinable()) t1.join();
    
    std::for_each(threads.begin(), threads.end(),
        std::mem_fn(&std::thread::join));

    return 0;
}
