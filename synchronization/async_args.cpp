#include <iostream>
#include <future>
#include <chrono>
#include <string>
#include <sstream>

class X {
 public:
    X():id(idGen++) {
        std::cout<<"X()"<<std::endl;
    }

    X(const X &){
        std::cout<<"X() copy"<<std::endl;
    }

    X(const X &&){
        std::cout<<"X() move"<<std::endl;
    }
    
    bool doStuff(std::string const &msg) {
        std::cout << "Object " <<id<<": "<<msg<<std::endl;
        return true;
    }

    bool operator()(std::string const &msg) {
        std::cout << "Op Object " <<id<<": "<<msg<<std::endl;
        return true;
    }
 private:
    int id;
    static int idGen;
};

int X::idGen = 0;



int main(int argc, char *argv[]) {
    X x;

    // pass arg by reference
    auto f1 = std::async(&X::doStuff, &x, std::string("by ref"));

    // creates temporary object
    auto f2 = std::async(&X::doStuff, x, std::string("temporary object"));
    
    // implementation chooses
    auto f3 = std::async(std::ref(x), std::string("std::ref"));

    auto r1 = f1.get();
    auto r2 = f2.get();
    auto r3 = f3.get(); 
    
    return 0;
}
