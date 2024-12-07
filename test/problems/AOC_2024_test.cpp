#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2024.h>
#include <incom_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2024 "../../data/AOC_2024"


TEST(A_2024_d1_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_1_1.txt";
    EXPECT_EQ(2430334, AOC2024::day1_1(pth.generic_string()));
}
TEST(A_2024_d1_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_1_1.txt";
    EXPECT_EQ(28786472, AOC2024::day1_2(pth.generic_string()));
}