#include <iostream>
#include <future>
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


int heavyWork(std::string msg) {
    std::cout << msg << " " << get_str_id() << std::endl;
    int cnt = 0;
    for (int i=0; i < 1000; i++)
        for (int j=0; j < 1000; j++)
            cnt += 1;
    return cnt;
}


int main(int argc, char *argv[]) {
    
    // run in a new thread
    auto f1 = std::async(std::launch::async, heavyWork,
        std::string("async"));
    // run in wait or get
    auto f2 = std::async(std::launch::deferred, heavyWork,
        std::string("deferred"));
    // implementation chooses
    auto f3 = std::async(std::launch::deferred | std::launch::async, 
        heavyWork, std::string("deferred or async"));

    auto r1 = f1.get();
    auto r2 = f2.get();
    auto r3 = f3.get(); 
    std::cout << "Result1: " << r1 << std::endl;
    std::cout << "Result2: " << r2 << std::endl;
    std::cout << "Result3: " << r3 << std::endl;

    return 0;
}
