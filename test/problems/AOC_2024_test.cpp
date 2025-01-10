#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2024.h>
#include <incom_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2024 "../../data/AOC_2024"
using namespace std::literals;


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
TEST(A_2024_d5_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_5_1.txt";
    EXPECT_EQ(5964, AOC2024::day5_1(pth.generic_string()));
}
TEST(A_2024_d5_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_5_1.txt";
    EXPECT_EQ(4719, AOC2024::day5_2(pth.generic_string()));
}
TEST(A_2024_d6_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_6_1.txt";
    EXPECT_EQ(5030, AOC2024::day6_1(pth.generic_string()));
}
TEST(A_2024_d6_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_6_1.txt";
    EXPECT_EQ(1928, AOC2024::day6_2(pth.generic_string()));
}
TEST(A_2024_d7_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_7_1.txt";
    EXPECT_EQ(3351424677624, AOC2024::day7_1(pth.generic_string()));
}
TEST(A_2024_d7_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_7_1.txt";
    EXPECT_EQ(204976636995111, AOC2024::day7_2(pth.generic_string()));
}
TEST(A_2024_d8_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_8_1.txt";
    EXPECT_EQ(247, AOC2024::day8_1(pth.generic_string()));
}
TEST(A_2024_d8_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_8_1.txt";
    EXPECT_EQ(861, AOC2024::day8_2(pth.generic_string()));
}
TEST(A_2024_d9_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_9_1.txt";
    EXPECT_EQ(6262891638328, AOC2024::day9_1(pth.generic_string()));
}
TEST(A_2024_d9_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_9_1.txt";
    EXPECT_EQ(6287317016845, AOC2024::day9_2(pth.generic_string()));
}
TEST(A_2024_d10_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_10_1.txt";
    EXPECT_EQ(719, AOC2024::day10_1(pth.generic_string()));
}
TEST(A_2024_d10_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_10_1.txt";
    EXPECT_EQ(1530, AOC2024::day10_2(pth.generic_string()));
}
TEST(A_2024_d11_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_11_1.txt";
    EXPECT_EQ(188902, AOC2024::day11_1(pth.generic_string()));
}
TEST(A_2024_d11_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_11_1.txt";
    EXPECT_EQ(223894720281135, AOC2024::day11_2(pth.generic_string()));
}
TEST(A_2024_d12_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_12_1.txt";
    EXPECT_EQ(1522850, AOC2024::day12_1(pth.generic_string()));
}
TEST(A_2024_d12_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_12_1.txt";
    EXPECT_EQ(953738, AOC2024::day12_2(pth.generic_string()));
}
TEST(A_2024_d13_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_13_1.txt";
    EXPECT_EQ(32067, AOC2024::day13_1(pth.generic_string()));
}
TEST(A_2024_d13_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_13_1.txt";
    EXPECT_EQ(92871736253789, AOC2024::day13_2(pth.generic_string()));
}
TEST(A_2024_d14_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_14_1.txt";
    EXPECT_EQ(230461440, AOC2024::day14_1(pth.generic_string(), 100));
}
TEST(A_2024_d14_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_14_1.txt";
    EXPECT_EQ(6668, AOC2024::day14_2(pth.generic_string()));
}
TEST(A_2024_d15_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_15_1.txt";
    EXPECT_EQ(1517819, AOC2024::day15_1(pth.generic_string()));
}
TEST(A_2024_d15_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_15_1.txt";
    EXPECT_EQ(1538862, AOC2024::day15_2(pth.generic_string()));
}
TEST(A_2024_d16_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_16_1.txt";
    EXPECT_EQ(94436, AOC2024::day16_1(pth.generic_string()));
}
TEST(A_2024_d16_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_16_1.txt";
    EXPECT_EQ(481, AOC2024::day16_2(pth.generic_string()));
}
TEST(A_2024_d17_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_17_1.txt";
    EXPECT_EQ("1,0,2,0,5,7,2,1,3"sv, AOC2024::day17_1(pth.generic_string()));
}
TEST(A_2024_d17_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_17_1.txt";
    EXPECT_EQ(265652340990875, AOC2024::day17_2(pth.generic_string()));
}
TEST(A_2024_d18_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_18_1.txt";
    EXPECT_EQ(268, AOC2024::day18_1(pth.generic_string()));
}
TEST(A_2024_d18_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_18_1.txt";
    EXPECT_EQ("64,11"sv, AOC2024::day18_2(pth.generic_string()));
}
TEST(A_2024_d19_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_19_1.txt";
    EXPECT_EQ(313, AOC2024::day19_1(pth.generic_string()));
}
TEST(A_2024_d19_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_19_1.txt";
    EXPECT_EQ(666491493769758, AOC2024::day19_2(pth.generic_string()));
}
TEST(A_2024_d20_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_20_1.txt";
    EXPECT_EQ(1518, AOC2024::day20_1(pth.generic_string()));
}
TEST(A_2024_d20_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_20_1.txt";
    EXPECT_EQ(1032257, AOC2024::day20_2(pth.generic_string()));
}
TEST(A_2024_d21_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_21_1.txt";
    EXPECT_EQ(132532, AOC2024::day21_1(pth.generic_string()));
}
TEST(A_2024_d21_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_21_1.txt";
    EXPECT_EQ(165644591859332, AOC2024::day21_2(pth.generic_string()));
}
TEST(A_2024_d22_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_22_1.txt";
    EXPECT_EQ(19458130434, AOC2024::day22_1(pth.generic_string()));
}
TEST(A_2024_d22_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_22_1.txt";
    EXPECT_EQ(2130, AOC2024::day22_2(pth.generic_string()));
}