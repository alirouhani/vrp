#ifndef PARAMS_H
#define PARAMS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct Params {
    int N; // Number of nodes
    int K; // Number of vehicles
    int vehicle_capacity; // Vehicle capacity
    std::vector<std::vector<int>> distance_matrix; // Distance matrix
    std::vector<double> demand; // Demand at each node
    Params() = default;
    void loadFromFile(const std::string& file_path) {
        std::string line;
        std::fstream myFile(file_path);
        if (myFile.is_open()) {
            int line_count = 0;
            while (std::getline(myFile, line)) {
                std::string::iterator it;
                if (line_count == 0) {
                    for (it = line.begin(); it != line.end(); ++it) {
                        std::string::iterator it_prime;
                        if (*it == 'n') {
                            std::string n = "";
                            for (it_prime = it; it_prime != line.end(); ++it_prime) {
                                if (*it_prime == 'n') continue;
                                if (*it_prime == '-') break;
                                n += *it_prime;
                            }
                            N = std::stoi(n);
                            demand.reserve(N);
                            distance_matrix.reserve(N);
                        }
                        if (*it == 'k') {
                            std::string k = "";
                            for (it_prime = it; it_prime != line.end(); ++it_prime) {
                                if (*it_prime == 'k') continue;
                                if (*it_prime == '-') break;
                                k += *it_prime;
                            }
                            K = std::stoi(k);
                        }
                    }
                } else if (line_count == 5) {
                    std::string cap = "";
                    for (it = line.begin(); it != line.end(); ++it) {
                        if (*it >= '0' && *it <= '9') cap += *it;
                    }
                    vehicle_capacity = std::stoi(cap);
                } else if (line_count >= 8 && line_count <= 8 + N - 1) {
                    // std::vector<int> inner_vector;
                    // inner_vector.reserve(N);
                } else if (line_count >= 8 + N + 1 && line_count <= 8 + 2*N) {
                }
                line_count += 1;
            }
            myFile.close();
        } else {
            throw std::runtime_error("Unable to open file: " + file_path);
        }
        // Implement file loading logic here
    }
};

#endif // PARAMS_H