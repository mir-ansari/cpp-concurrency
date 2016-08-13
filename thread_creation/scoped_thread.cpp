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

class scoped_thread {
 public:
    explicit scoped_thread(std::thread t_):
            t(std::move(t_)) {
        if (!t.joinable()) {
            throw std::logic_error("No thread");
        }
    }

    ~scoped_thread() {
        t.join();
    }

    // not allowed to be copied or assigned
    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;

 private:
    std::thread t;
};

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
    scoped_thread t1(std::thread(w1, std::string("thread A")));

    worker w2;
    scoped_thread t2(std::thread(w2, std::string("thread A")));

    std::cout << "main ended" << std::endl;

    return 0;
}
