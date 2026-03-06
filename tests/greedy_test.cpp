#include <gtest/gtest.h>
#include "../include/Params.h"
#include "../include/greedy_solver.h"

TEST(RouteTest, AllVisited) {
    Params params;
    params.loadFromFile("test_data.vrp");
    greedy_result gy_res;
    gy_res = greedy(params);
    EXPECT_EQ(gy_res.visited_nodes, params.N);
}

TEST(SolutionTest, TrueObjetive) {
    Params params;
    params.loadFromFile("test_data.vrp");
    greedy_result gy_res;
    gy_res = greedy(params);
    EXPECT_EQ(gy_res.obj_val, 450);
    EXPECT_EQ(gy_res.visited_nodes, params.N);
}
