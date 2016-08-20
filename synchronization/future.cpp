#include <iostream>
#include <future>
#include <chrono>

int heavyWorkArgs(int m) {
    int cnt = 0;
    for (int i=0; i < 1000; i++)
        for (int j=0; j < 1000; j++)
            cnt += m;
    return cnt;
}

int heavyWork() {
    int cnt = 0;
    for (int i=0; i < 1000; i++)
        for (int j=0; j < 1000; j++)
            cnt++;
    return cnt;
}

void doStuff() {
    std::cout << "Doing other work" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char *argv[]) {
    // starts an asynchronous task
    // the task is run in another thread
    std::future<int> result = std::async(heavyWork);
    std::future<int> resultArgs = std::async(heavyWorkArgs, 2);
    doStuff();
    // get result from future of the task
    // this is a blocking call
    std::cout << "The answer is: " << result.get() << std::endl;
    std::cout << "The answer with Args is: " << resultArgs.get() << std::endl;
    return 0;
}
