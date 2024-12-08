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
TEST(A_2024_d2_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_2_1.txt";
    EXPECT_EQ(269, AOC2024::day2_1(pth.generic_string()));
}
TEST(A_2024_d2_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_2_1.txt";
    EXPECT_EQ(337, AOC2024::day2_2(pth.generic_string()));
}
TEST(A_2024_d3_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_3_1.txt";
    EXPECT_EQ(157621318, AOC2024::day3_1(pth.generic_string()));
}
TEST(A_2024_d3_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_3_1.txt";
    EXPECT_EQ(79845780, AOC2024::day3_2(pth.generic_string()));
}
TEST(A_2024_d4_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_4_1.txt";
    EXPECT_EQ(2434, AOC2024::day4_1(pth.generic_string()));
}
TEST(A_2024_d4_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_4_1.txt";
    EXPECT_EQ(1835, AOC2024::day4_2(pth.generic_string()));
}
TEST(A_2024_d4_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_4_1.txt";
    EXPECT_EQ(1835, AOC2024::day4_3(pth.generic_string()));
}