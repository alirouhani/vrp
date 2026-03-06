#include <gtest/gtest.h>
#include <fstream>
#include "../include/Params.h"
#include "../include/VrpSolver.h"

TEST(FileContentTest, IsFileEmpty) {
    std::ifstream file("test_data.vrp");
    
    ASSERT_TRUE(file.is_open()) << "Could not open file.";
    EXPECT_NE(file.peek(), std::ifstream::traits_type::eof());
}

TEST(ParamsTest, IsParseTrue) {
    Params params;
    params.loadFromFile("test_data.vrp");
    EXPECT_EQ(params.N, 16);
    EXPECT_EQ(params.K, 8);
    EXPECT_EQ(params.vehicle_capacity, 35);
    for (size_t i = 0; i < params.N; ++i) {
        for (size_t j = 0; j < i; ++j) {
            ASSERT_NEAR(
                params.distance_matrix[params.index(i,j)],
                params.distance_matrix[params.index(i,j)],
                1e-6
            );
        }
    }
    double obj_val = SolveCVRP_Routing(params);
    EXPECT_EQ(obj_val, 450);
}