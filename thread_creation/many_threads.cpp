#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <vector>

std::string get_str_id() {
	auto myid = std::this_thread::get_id();
	std::stringstream ss;
	ss << "[id="<<myid<<"] ";
	return ss.str();
}

class worker {
	std::thread thd;
	std::string name;
public:
	worker(std::string name):name(name) {
		thd = std::thread(&worker::execute, this);
	}

	void execute() const {
		std::cout<<get_str_id()<<name<<std::endl;
		heavy_work();
	}
	
	void heavy_work() const {
		std::cout<<get_str_id()<<"in heavy work"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	void join() {
		thd.join();
	}

};

void do_work(int id) {
	std::cout<<get_str_id()<<"in do work, id="<<id<<std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
}


int main() {
	std::cout<<get_str_id()<<"Main thread "<<std::endl;

	std::vector<std::thread> threads;
	for (int i=0; i<3; i++) {
		threads.push_back(std::thread(do_work, i));
	}

	std::for_each(threads.begin(), threads.end(), 
		std::mem_fn(&std::thread::join));
	

	std::vector<worker> workers;
	for (int i=0; i<3; i++) {
		workers.push_back(worker(std::string("thread:") + std::to_string(i)));
	}

	std::for_each(workers.begin(), workers.end(), 
		std::mem_fn(&worker::join));

	std::cout<<"main ended"<<std::endl;

	return 0;
}
