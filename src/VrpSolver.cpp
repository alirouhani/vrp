#include "../include/VrpSolver.h"
#include "ortools/linear_solver/linear_solver.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace operations_research;

double GetDistance(const Params& p, int i, int j) {
    if (i == j) return 0.0;
    return p.distance_matrix[p.index(i, j)];
}

double SolveVRPWithMTZ(const Params& p) {
    // Create the linear solver with the SCIP backend
    std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
    if (!solver) {
        std::cerr << "SCIP solver not available." << std::endl;
        return -1;
    }

    int N = p.N;
    int K = p.K;

    // -------------------------------------------------------------------------
    // 1. Variables
    // -------------------------------------------------------------------------
    // x[i][j][k]: 1 if vehicle k travels from i to j
    std::vector<std::vector<std::vector<const MPVariable*>>> x(
        N, std::vector<std::vector<const MPVariable*>>(
            N, std::vector<const MPVariable*>(K, nullptr)));

    // y[i][k]: 1 if node i is visited by vehicle k
    std::vector<std::vector<const MPVariable*>> y(
        N, std::vector<const MPVariable*>(K, nullptr));

    // u[i][k]: Accumulated load of vehicle k after visiting node i
    std::vector<std::vector<const MPVariable*>> u(
        N, std::vector<const MPVariable*>(K, nullptr));

    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < K; ++k) {
            y[i][k] = solver->MakeIntVar(0, 1, "y_" + std::to_string(i) + "_" + std::to_string(k));
            u[i][k] = solver->MakeNumVar(p.demand[i], p.vehicle_capacity, "u_" + std::to_string(i) + "_" + std::to_string(k));
            
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    x[i][j][k] = solver->MakeIntVar(0, 1, "x_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k));
                }
            }
        }
    }

    // -------------------------------------------------------------------------
    // 2. Objective: Minimize total travel distance
    // -------------------------------------------------------------------------
    MPObjective* const objective = solver->MutableObjective();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i != j) {
                double dist = GetDistance(p, i, j);
                for (int k = 0; k < K; ++k) {
                    objective->SetCoefficient(x[i][j][k], dist);
                }
            }
        }
    }
    objective->SetMinimization();

    // -------------------------------------------------------------------------
    // 3. Constraints
    // -------------------------------------------------------------------------
    
    // Constraint: Each customer (excluding depot) must be visited exactly once
    for (int i = 1; i < N; ++i) {
        MPConstraint* c = solver->MakeRowConstraint(1, 1, "visit_" + std::to_string(i));
        for (int k = 0; k < K; ++k) {
            c->SetCoefficient(y[i][k], 1);
        }
    }

    // Constraint: Capacity constraint per vehicle
    for (int k = 0; k < K; ++k) {
        MPConstraint* c = solver->MakeRowConstraint(-solver->infinity(), p.vehicle_capacity, "cap_" + std::to_string(k));
        for (int i = 1; i < N; ++i) {
            c->SetCoefficient(y[i][k], p.demand[i]);
        }
    }

    // Constraint: Each vehicle starts at the depot
    MPConstraint* depot_c = solver->MakeRowConstraint(K, K, "depot_start");
    for (int k = 0; k < K; ++k) {
        depot_c->SetCoefficient(y[0][k], 1);
    }

    // Flow & Continuity Constraints
    for (int k = 0; k < K; ++k) {
        for (int i = 0; i < N; ++i) {
            // sum(x[i][j][k]) == sum(x[j][i][k])
            MPConstraint* flow = solver->MakeRowConstraint(0, 0, "flow_" + std::to_string(i) + "_" + std::to_string(k));
            // sum(x[i][j][k]) == y[i][k]
            MPConstraint* link = solver->MakeRowConstraint(0, 0, "link_" + std::to_string(i) + "_" + std::to_string(k));
            
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    flow->SetCoefficient(x[i][j][k], 1);
                    flow->SetCoefficient(x[j][i][k], -1);
                    link->SetCoefficient(x[i][j][k], 1);
                }
            }
            link->SetCoefficient(y[i][k], -1);
        }
    }

    // MTZ Constraints: u[j,k] >= u[i,k] + demands[j] - Q*(1 - x[i,j,k])
    for (int k = 0; k < K; ++k) {
        for (int i = 1; i < N; ++i) {
            for (int j = 1; j < N; ++j) {
                if (i != j) {
                    double rhs = p.vehicle_capacity - p.demand[j];
                    MPConstraint* mtz = solver->MakeRowConstraint(-solver->infinity(), rhs, "");
                    mtz->SetCoefficient(u[i][k], 1);
                    mtz->SetCoefficient(u[j][k], -1);
                    mtz->SetCoefficient(x[i][j][k], p.vehicle_capacity);
                }
            }
        }
    }

    // -------------------------------------------------------------------------
    // 4. Solve and Extract Routes
    // -------------------------------------------------------------------------
    solver->EnableOutput();
    std::cout << "Solving model..." << std::endl;
    MPSolver::ResultStatus result_status = solver->Solve();

    if (result_status == MPSolver::OPTIMAL) {
        std::cout << "Solution found!" << std::endl;
        std::cout << "Objective value: " << objective->Value() << "\n" << std::endl;

        for (int k = 0; k < K; ++k) {
            std::cout << "Vehicle " << k << " Route: ";
            std::vector<std::pair<int, int>> route;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (i != j && x[i][j][k]->solution_value() > 0.5) {
                        route.push_back({i, j});
                    }
                }
            }
            for (const auto& edge : route) {
                std::cout << "(" << edge.first << " -> " << edge.second << ") ";
            }
            std::cout << "\n";
        }
        return objective->Value();
    } else {
        std::cout << "The solver could not find an optimal solution." << std::endl;
    }
    return -1;
}
