#ifndef VRP_SOLVER_H
#define VRP_SOLVER_H

#include "Params.h"

inline double GetDistance(const Params& p, int i, int j) {
    if (i == j) return 0.0;
    return p.distance_matrix[p.index(i, j)];
}

double SolveCVRP_Routing(const Params& p);

#endif // VRP_SOLVER_H