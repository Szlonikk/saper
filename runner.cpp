#include <iostream>
#include <cstdlib>
int main(){
    std::string build_dir = "build";

    std::string command = "cd " + build_dir;
    std::system(command.c_str());

    command = "cmake ..";
    std::system(command.c_str());

}   