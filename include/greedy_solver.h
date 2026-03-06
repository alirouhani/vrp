#ifndef GREEDY_SOLVER_H
#define GREEDY_SOLVER_H

#include "Params.h"
#include <vector>
#include <cstdint>
#include <limits>

struct greedy_result {
    int visited_nodes;
    double obj_val;
};

[[nodiscard]] inline greedy_result greedy(const Params& p) {
    // Edge case: No customers to visit
    if (p.N <= 1) return {p.N, 0.0}; 

    std::vector<uint8_t> visited(p.N, 0);
    visited[0] = 1; 

    int customers_visited = 0;
    const int total_customers = p.N - 1;
    double total_distance = 0.0;

    for (int k = 0; k < p.K; ++k) {
        // Early exit: Stop dispatching vehicles if all customers are served
        if (customers_visited == total_customers) {
            break;
        }

        int remain_cap = p.vehicle_capacity;
        int current_node = 0; 

        while (true) {
            double min_distance = std::numeric_limits<double>::max();
            int best_index = -1;

            for (int j = 1; j < p.N; ++j) {
                if (!visited[j] && p.demand[j] <= remain_cap) {
                    double d = p.distance_matrix[p.index(current_node, j)];
                    if (d < min_distance) {
                        min_distance = d;
                        best_index = j;
                    }
                }
            }

            if (best_index == -1) {
                // No valid customers left for this vehicle's capacity; return to depot
                total_distance += p.distance_matrix[p.index(current_node, 0)];
                break; 
            }

            // Visit the best customer found
            visited[best_index] = 1;
            customers_visited++;
            total_distance += min_distance;
            remain_cap -= p.demand[best_index];
            current_node = best_index;

            // Early exit inner loop: if this was the last customer, return to depot immediately
            if (customers_visited == total_customers) {
                total_distance += p.distance_matrix[p.index(current_node, 0)];
                break;
            }
        }
    }

    // Return customers visited + 1
    return {customers_visited + 1, total_distance};
}

#endif