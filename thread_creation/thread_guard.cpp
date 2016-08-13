// "Copyright [2016] <Asit Dhal, dhal.asitk@gmail.com>"

#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>

// returns thread id in std::string
std::string get_str_id() {
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << "[id=" << myid << "] ";
    return ss.str();
}

// a thread_guard object created for a thread will wait till the
// thread finishes. the programmer should create an object for a
// thread and forget. the control of the calling thread can't leave
// the current block, until thread finishes.
class thread_guard {
 public:
    explicit thread_guard(std::thread& t_): t(t_) {}

    // the guarded thread will be waited
    ~thread_guard() {
        if (t.joinable()) {
            t.join();
        }
    }

    // not allowed to be copied or assigned
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;

 private:
    std::thread& t;
};

// a worker class
class worker {
 public:
    void operator()(std::string msg) const {
        std::cout << get_str_id() << msg << std::endl;
        heavy_work();
    }

    void heavy_work() const {
        std::cout << get_str_id() << "in heavy work" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
};


int main() {
    std::cout << get_str_id() << "Main thread " << std::endl;

    worker w1;
    std::thread t1(w1, std::string("thread 1"));
    thread_guard g1(t1);

    worker w2;
    std::thread t2(w1, std::string("thread 2"));
    thread_guard g2(t2);

    std::cout << "main ended" << std::endl;

    return 0;
}
