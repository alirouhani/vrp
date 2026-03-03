#include <gtest/gtest.h>
#include "../include/Params.h"

TEST(FunctionTest, PrintMessage) {
    testing::internal::CaptureStdout();
    Params params;
    params.print_message("Hello, Test!");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Hello, Test!\n");
}

TEST(print_message, EmptyString) {
    testing::internal::CaptureStdout();
    Params params;
    params.print_message("");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "\n");
}
