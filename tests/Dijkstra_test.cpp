#include <gtest/gtest.h>
#include "../include/Params.h"
#include "../include/Dijkstra.h"

TEST(ShortestPathTest, AllVisited) {
    Params params;
    params.loadFromFile("test_data.vrp");
    int visited_customers = dijkstra(params);
    EXPECT_EQ(visited_customers+1, params.N);
}