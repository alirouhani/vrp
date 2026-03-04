#include "include/Params.h"
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::string file_path = argv[1];
    Params params;
    params.print_message("Hello, World!");
    return 0;
}