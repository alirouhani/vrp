#include <gtest/gtest.h>
#include "../include/function.h"

TEST(FunctionTest, PrintMessage) {
    testing::internal::CaptureStdout();
    print_message("Hello, Test!");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Hello, Test0!\n");
}

TEST(print_message, EmptyString) {
    testing::internal::CaptureStdout();
    print_message("");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "\n");
}
