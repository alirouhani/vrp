#ifndef PARAMS_H
#define PARAMS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdexcept>

struct Params {
    int N = 0;
    int K = 0;
    int vehicle_capacity = 0;

    std::vector<double> distance_matrix;
    std::vector<double> demand;

    std::vector<double> x_coord;
    std::vector<double> y_coord;

    // triangle index
    inline size_t index(int i, int j) const {
        if (i < j) std::swap(i,j);
        return (size_t)i * (i - 1) / 2 + j;
    }

    void loadFromFile(const std::string& file_path) {

        std::ifstream file(file_path);
        if (!file)
            throw std::runtime_error("Cannot open file");

        std::string line;
        int line_count = 0;

        while (getline(file, line)) {

            if (line_count == 0) {
                auto pos = line.find(':');
                std::string instance = line.substr(pos + 1);
                size_t npos = instance.find("-n");
                size_t kpos = instance.find("-k");

                N = std::stoi(instance.substr(npos + 2, kpos - (npos + 2)));
                K = std::stoi(instance.substr(kpos + 2));

                demand.resize(N);
                x_coord.resize(N);
                y_coord.resize(N);

                // allocate triangle matrix
                distance_matrix.resize((size_t)N*(N-1)/2);

            }

            else if (line_count == 5) {

                auto pos = line.find(':');
                vehicle_capacity = std::stoi(line.substr(pos + 1));
            }

            else if (line_count >= 7 && line_count < 7 + N) {

                int i;
                double x,y;

                std::stringstream ss(line);
                ss >> i >> x >> y;

                i--;

                x_coord[i] = x;
                y_coord[i] = y;

                // compute distances with previous nodes
                for(std::size_t j = 0; j < i; j++) {

                    double d = std::hypot(
                        x - x_coord[j],
                        y - y_coord[j]
                    );

                    distance_matrix[index(i,j)] = d;
                }
            }

            else if (line_count >= 8 + N && line_count < 8 + 2*N) {

                int i;
                double dem;

                std::stringstream ss(line);
                ss >> i >> dem;

                demand[i-1] = dem;
            }

            line_count++;
        }
    }
};

#endif // PARAMS_H