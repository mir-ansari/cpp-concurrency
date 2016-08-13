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

class worker {
 public:
    void operator()() const {
        std::cout << get_str_id() << "in operator" << std::endl;
        heavy_work();
    }

    void heavy_work() const {
        std::cout << get_str_id() << "in heavy work" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void parametric_work(std::string msg) {
        std::cout << get_str_id() << "Msg: " << msg << std::endl;
        heavy_work();
    }
};

class worker2 {
 private:
    std::thread thd1, thd2;
 public:
    worker2() {
        thd1 = std::thread(&worker2::heavy_work, this);
        thd2 = std::thread(&worker2::parametric_work,
            this, std::string("woolala"));
    }

    void heavy_work() const {
        std::cout << get_str_id() << "in heavy work" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void parametric_work(std::string msg) {
        std::cout << get_str_id() << "Msg: " << msg <<std::endl;
        heavy_work();
    }

    ~worker2() {
        if (thd1.joinable()) thd1.join();
        if (thd2.joinable()) thd2.join();
    }
};

int main() {
    worker w1;
    std::thread t1(w1);

    worker w2;
    std::thread t2(&worker::parametric_work, w2, std::string("Asit Dhal") );

    std::cout << get_str_id() << "Main thread " << std::endl;

    worker2 work2;

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();

    std::cout << "main ended" << std::endl;
}

