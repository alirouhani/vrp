#ifndef GREEDY_SOLVER_H
#define GREEDY_SOLVER_H

#include "Params.h"
#include <vector>
#include <algorithm>
#include <limits>

struct greedy_result {
    int visited_nodes;
    double obj_val;
};

inline greedy_result greedy(const Params& p) {
    greedy_result res = {0, 0.0};
    std::vector<bool> visited(p.N, false);
    double total_distance = 0;

    for (int k = 0; k < p.K; ++k) {
        int remain_cap = p.vehicle_capacity;
        int current_node = 0; // Assuming 0 is the depot
        visited[0] = true;

        while (true) {
            double min_distance = std::numeric_limits<double>::max();
            int best_index = -1;

            for (int j = 0; j < p.N; ++j) {
                if (!visited[j] && p.demand[j] <= remain_cap) {
                    double d = p.distance_matrix[p.index(current_node, j)];
                    if (d < min_distance) {
                        min_distance = d;
                        best_index = j;
                    }
                }
            }

            if (best_index == -1) {
                // Return to depot
                total_distance += p.distance_matrix[p.index(current_node, 0)];
                break; 
            }

            visited[best_index] = true;
            total_distance += min_distance;
            remain_cap -= p.demand[best_index];
            current_node = best_index;
        }
    }

    res.obj_val = total_distance;
    res.visited_nodes = std::count(visited.begin(), visited.end(), true);
    return res;
}

#endif