#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>

std::string get_str_id() {
	auto myid = std::this_thread::get_id();
	std::stringstream ss;
	ss << "[id="<<myid<<"] ";
	return ss.str();
}


class thread_guard {
	std::thread& t;

public:
	explicit thread_guard(std::thread& t_): t(t_) {}


	//the guarded thread will be waited 
	~thread_guard() {
		if(t.joinable()) {
			t.join();
		}
	}

	//not allowed to be copied or assigned
	thread_guard(thread_guard const&) = delete;
	thread_guard& operator=(thread_guard const&) = delete;
};

class worker {
public:
	void operator()(std::string msg) const {
		std::cout<<get_str_id()<<msg<<std::endl;
		heavy_work();
	}

	
	void heavy_work() const {
		std::cout<<get_str_id()<<"in heavy work"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

};




int main() {
	worker w1;
	std::thread t1(w1, std::string("thread 1"));
	thread_guard g1(t1);

	worker w2;
	std::thread t2(w1, std::string("thread 2"));
	thread_guard g2(t2);

	return 0;
}