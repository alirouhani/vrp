#include <gtest/gtest.h>
#include <fstream>
#include "../include/Params.h"

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
}