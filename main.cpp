#include "include/Params.h"
#include "include/Dijkstra.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_instance.txt>" << std::endl;
        return 1;
    }

    try {
        Params p;
        p.loadFromFile(argv[1]);
        std::cout << "Loaded instance with " << p.N << " nodes and " 
                  << p.K << " vehicles. Capacity: " << p.vehicle_capacity << std::endl;
        
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}