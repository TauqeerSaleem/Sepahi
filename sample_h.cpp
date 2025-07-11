#include <string>

std::string say_hello(int number_of_times) {
    std::string out;
    for (int i = 0; i < number_of_times; i++){
        out += "Hello World ";
    }

    return out;
}
