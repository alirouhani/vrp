#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Params.h"
#include <vector>
#include <queue>
#include <iostream>
#include <limits>
#include <cstdint>
#include <algorithm>

// A State represents a partial route: the sequence of visited nodes (route),
// remaining vehicle capacity, and accumulated travel cost (distance) so far.
struct State {
    std::vector<int> route;
    int remaining_capacity;
    double cost;

    State() : route(), remaining_capacity(0), cost(0.0) {}
    State(std::vector<int> r, int cap, double c) : route(std::move(r)), remaining_capacity(cap), cost(c) {}
};

// Comparator for priority_queue to make it a min-heap on State.cost
struct CompareCost {
    bool operator()(State const& a, State const& b) const {
        return a.cost > b.cost; // smaller cost has higher priority
    }
};

inline bool contains_node(const std::vector<int>& v, int node) {
    return std::find(v.begin(), v.end(), node) != v.end();
}

int dijkstra(const Params& p) {
    // Edge case: no customers (only depot)
    if (p.N <= 1) {
        std::cout << "No customers to serve (N <= 1)." << std::endl;
        return -1;
    }

    const int total_customers = p.N - 1; // customers are nodes 1..N-1
    int customers_visited = 0;
    double total_distance = 0.0;

    // Global visited flags: index 0 is depot (we don't count it in customers)
    std::vector<uint8_t> visited_global(p.N, 0);
    visited_global[0] = 1;

    // For each vehicle, build one route (best-first)
    for (int vehicle = 0; vehicle < p.K; ++vehicle) {
        if (customers_visited >= total_customers) break; // all served

        // Priority queue of partial routes (best-first by accumulated cost)
        std::priority_queue<State, std::vector<State>, CompareCost> pq;

        // Start from depot, full capacity, zero cost
        pq.emplace(std::vector<int>{0}, p.vehicle_capacity, 0.0);

        bool route_finalized = false;
        // Expand states until we finalize a route or no states left
        while (!pq.empty() && !route_finalized) {
            State current = pq.top();
            pq.pop();

            int current_node = current.route.back();
            bool expanded = false;

            // Try to expand to any not-yet-globally-served customer j
            for (int j = 1; j < p.N; ++j) {
                if (visited_global[j]) continue;                    // already served by previous routes
                if (contains_node(current.route, j)) continue;     // already in this partial route
                if (p.demand[j] > current.remaining_capacity) continue; // capacity constraint

                // feasible expansion
                expanded = true;
                double d = p.distance_matrix[p.index(current_node, j)];
                std::vector<int> next_route = current.route;
                next_route.push_back(j);
                pq.emplace(std::move(next_route), current.remaining_capacity - p.demand[j], current.cost + d);
            }

            // If we could not expand this state, finalize it as the route for this vehicle
            if (!expanded) {
                // Add return-to-depot distance
                double return_d = p.distance_matrix[p.index(current_node, 0)];
                total_distance += current.cost + return_d;

                // Mark customers in this route as visited (skip depot at index 0)
                for (std::size_t idx = 1; idx < current.route.size(); ++idx) {
                    int cust = current.route[idx];
                    if (!visited_global[cust]) {
                        visited_global[cust] = 1;
                        ++customers_visited;
                    }
                }

                route_finalized = true;
                // finished with this vehicle
            }
            // otherwise, continue popping next-best partial route
        }

        if (customers_visited >= total_customers) break; // all served
    }

    std::cout << "Number of visited customers: " << customers_visited
              << " with total distance " << total_distance << std::endl;
    
    return customers_visited;
}

#endif // DIJKSTRA_H