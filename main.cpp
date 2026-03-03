#include "include/Params.h"
#include <iostream>
int main(int argc, char* argv[]) {
    std::string file_path = argv[1];
    Params params;
    params.print_message("Hello, World!");
    return 0;
}