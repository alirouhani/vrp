// Example: Solve CVRP with OR-Tools Routing (C++)
// Adapt to your Params struct (distance_matrix, demand vector, vehicle_capacity, K, N).
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"
#include "absl/time/time.h"
#include "../include/Vrp_Solver.h"
#include <cmath>
#include <iostream>

using namespace operations_research;

double SolveCVRP_Routing(const Params& p) {
  const int num_nodes = p.N;
  const int num_vehicles = p.K;
  const int depot = 0;

  // Index manager and routing model
  RoutingIndexManager manager(
    num_nodes,
    num_vehicles,
    RoutingIndexManager::NodeIndex(0)
  );
  RoutingModel routing(manager);

const int transit_callback_index = routing.RegisterTransitCallback(
    [&p, &manager](int64_t from_index, int64_t to_index) -> int64_t {
        int from = manager.IndexToNode(from_index).value();
        int to   = manager.IndexToNode(to_index).value();
        double d = GetDistance(p, from, to);
        return static_cast<int64_t>(std::llround(d * 1000.0));
    });

  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

  // Demand callback
  const int demand_callback_index = routing.RegisterUnaryTransitCallback(
      [&p, &manager](int64_t from_index) -> int64_t {
        int node = manager.IndexToNode(from_index).value();
        return static_cast<int64_t>(p.demand[node]);
      });

  // Vehicle capacities vector
  std::vector<int64_t> vehicle_capacities(num_vehicles, static_cast<int64_t>(p.vehicle_capacity));

  // Add capacity dimension (name "Capacity")
  routing.AddDimensionWithVehicleCapacity(
      demand_callback_index,
      /*slack_max=*/0,
      vehicle_capacities,
      /*start_cumul_to_zero=*/true,
      "Capacity");

  // OPTIONAL: If you want to forbid empty routes or force exactly used vehicles, use disjunctions
  // or other constraints. By default unused vehicles allowed.

  // Set search parameters
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
  searchParameters.set_first_solution_strategy(FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION);
//   searchParameters.set_first_solution_strategy(FirstSolutionStrategy::PATH_CHEAPEST_ARC);
  searchParameters.set_local_search_metaheuristic(LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
  searchParameters.mutable_time_limit()->set_seconds(10); // 10 seconds; adjust as needed
  searchParameters.set_log_search(true);
  
  // Solve
  const Assignment* solution = routing.SolveWithParameters(searchParameters);
  if (!solution) {
    std::cout << "No solution found by routing solver." << std::endl;
    return -1.0;
  }

  // Extract solution and compute objective (remember to divide scale factor if used)
  long long total_cost = 0;
  for (int vehicle_id = 0; vehicle_id < num_vehicles; ++vehicle_id) {
    int64_t index = routing.Start(vehicle_id);
    if (routing.IsEnd(solution->Value(routing.NextVar(index)))) {
      // route is empty if Start leads directly to End
      continue;
    }
    std::cout << "Vehicle " << vehicle_id << " route: ";
    while (!routing.IsEnd(index)) {
      int node = manager.IndexToNode(index).value();
      std::cout << node << " -> ";
      int64_t next_index = solution->Value(routing.NextVar(index));
      total_cost += routing.GetArcCostForVehicle(index, next_index, vehicle_id);
      index = next_index;
    }
    std::cout << manager.IndexToNode(index) << "\n"; // depot at end
  }

  // If you scaled distances by 1000, divide back:
  double cost = static_cast<double>(total_cost) / 1000.0;
  std::cout << "Objective (approx): " << cost << std::endl;
  return cost;
}