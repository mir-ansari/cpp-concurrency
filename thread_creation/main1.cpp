#include <thread>
#include <iostream>
#include <string>

//thread function, which doesn't take any argument
void worker() {
	std::cout<<"worker thread"<<std::endl;
}

//thread function, which takes a string argument
void worker_args(std::string msg) {
	std::cout<<"worker thread with message: "<<msg<<std::endl;
}

void worker_from_lambda1()  {
	std::cout<<"called from lambda: "<<std::endl;
}

void worker_from_lambda2(std::string msg)  {
	std::cout<<"called from lambda: "<<msg<<std::endl;
}


int main(int argc, char* argv[]) {
	std::thread t1(worker);
	
	std::thread t2(worker_args, std::string("Asit Dhal"));

	std::thread t3([] {
		worker_from_lambda1();
		worker_from_lambda2(std::string("wooo lalala"));
		});
	
	std::cout<<"from main"<<std::endl;	

	if(t1.joinable()) t1.join();
	if(t2.joinable()) t2.join();
	if(t3.joinable()) t3.join();

	return 0;
}