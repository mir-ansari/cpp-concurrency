#include <thread>
#include <mutex>
#include <memory>
#include <iostream>


class Singleton {
 public:
    ~Singleton() {
        std::cout << "Destructor called" << std::endl;
    }
    static Singleton& getInstance() {
        std::call_once(
            m_initializedFlag,
            []() { Singleton::m_instance.reset(new Singleton); });
        return *m_instance;
    }
    void doWork() {
        std::cout << "I am Donald Trump." << std::endl;
    }

 private:
    Singleton() {
        std::cout << "Constructor called" << std::endl;
    }

 private:
    // flag to check if call_once() has been already called.
    static std::once_flag m_initializedFlag;
    static std::unique_ptr<Singleton> m_instance;
};

std::once_flag Singleton::m_initializedFlag;;
std::unique_ptr<Singleton> Singleton::m_instance;


void worker() {
    Singleton::getInstance().doWork();
}

int main() {
    Singleton::getInstance().doWork();
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);
    Singleton::getInstance().doWork();

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();

    return 0;
}
