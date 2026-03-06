#ifndef VRP_SOLVER_H
#define VRP_SOLVER_H

#include "Params.h"

// Helper to safely fetch distances using the 1D triangle matrix
double GetDistance(const Params& p, int i, int j);

// Formulates and solves the VRP using the MTZ constraints via OR-Tools
double SolveVRPWithMTZ(const Params& p);

#endif // VRP_SOLVER_H