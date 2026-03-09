# Vehicle Routing Problem (VRP)

A lightweight C++ implementation for experimenting with algorithms for the **Vehicle Routing Problem (VRP)**.

The project focuses on **simple and transparent algorithmic implementations** for studying routing strategies and graph-based optimization methods.

---

## Implemented Algorithms

### Greedy Heuristic

A constructive heuristic that iteratively selects the **nearest unvisited customer**.

At each step the vehicle moves to the node with the **minimum travel distance** from the current location.

Characteristics:

- Very fast
- Easy to implement
- Provides a baseline solution for VRP experiments

---

### Dijkstra-based Search

A shortest-path approach adapted for VRP exploration.

The algorithm uses **Dijkstra's method** to evaluate paths and maintain the best partial routes according to accumulated travel cost.

This approach allows systematic exploration of feasible routing states while maintaining optimality with respect to the path cost metric.

---

## Purpose
This repository is intended for:
1. Experimentation with VRP algorithms
2. Studying heuristic routing strategies
3. Testing graph search approaches for routing problems
4. The implementation prioritizes clarity and algorithmic transparency over framework complexity.
