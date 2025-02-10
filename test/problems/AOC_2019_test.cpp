#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2019.h>
#include <incom_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2018 "../../data/AOC_2019"


TEST(A_2019_d1_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_1_1.txt";
    EXPECT_EQ(3323874, AOC2019::day1_1(pth.generic_string()));
}
TEST(A_2019_d1_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_1_1.txt";
    EXPECT_EQ(4982961, AOC2019::day1_2(pth.generic_string()));
}
TEST(A_2019_d2_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_2_1.txt";
    EXPECT_EQ(3850704, AOC2019::day2_1(pth.generic_string()));
}
TEST(A_2019_d2_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_2_1.txt";
    EXPECT_EQ(6718, AOC2019::day2_2(pth.generic_string()));
}
TEST(A_2019_d3_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_3_1.txt";
    EXPECT_EQ(2427, AOC2019::day3_1(pth.generic_string()));
}
TEST(A_2019_d3_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_3_1.txt";
    EXPECT_EQ(27890, AOC2019::day3_2(pth.generic_string()));
}
TEST(A_2019_d4_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_4_1.txt";
    EXPECT_EQ(1650, AOC2019::day4_1(pth.generic_string()));
}
TEST(A_2019_d4_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_4_1.txt";
    EXPECT_EQ(1129, AOC2019::day4_2(pth.generic_string()));
}
TEST(A_2019_d5_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_5_1.txt";
    EXPECT_EQ(11193703, AOC2019::day5_1(pth.generic_string(), 1));
}
TEST(A_2019_d5_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_5_1.txt";
    EXPECT_EQ(12410607, AOC2019::day5_2(pth.generic_string(), 5));
}
TEST(A_2019_d6_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_6_1.txt";
    EXPECT_EQ(223251, AOC2019::day6_1(pth.generic_string()));
}
TEST(A_2019_d6_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_6_1.txt";
    EXPECT_EQ(430, AOC2019::day6_2(pth.generic_string()));
}
TEST(A_2019_d7_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_7_1.txt";
    EXPECT_EQ(38834, AOC2019::day7_1(pth.generic_string()));
}
TEST(A_2019_d7_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_7_1.txt";
    EXPECT_EQ(69113332, AOC2019::day7_2(pth.generic_string()));
}
TEST(A_2019_d8_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_8_1.txt";
    EXPECT_EQ(1703, AOC2019::day8_1(pth.generic_string(), {25, 6}));
}
TEST(DISABLED_A_2019_d8_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_8_1.txt";
    EXPECT_EQ("HCGFE", AOC2019::day8_2(pth.generic_string(), {25, 6}));
}
TEST(A_2019_d9_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_9_1.txt";
    EXPECT_EQ(2204990589, AOC2019::day9_1(pth.generic_string(), 1));
}
TEST(A_2019_d9_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_9_1.txt";
    EXPECT_EQ(50008, AOC2019::day9_1(pth.generic_string(), 2));
}
TEST(A_2019_d10_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_10_1.txt";
    EXPECT_EQ(282, AOC2019::day10_1(pth.generic_string()));
}
TEST(A_2019_d10_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_10_1.txt";
    EXPECT_EQ(1008, AOC2019::day10_2(pth.generic_string()));
}
TEST(A_2019_d11_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_11_1.txt";
    EXPECT_EQ(1709, AOC2019::day11_1(pth.generic_string()));
}
TEST(A_2019_d12_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_12_1.txt";
    EXPECT_EQ(7636, AOC2019::day12_1(pth.generic_string()));
}
TEST(A_2019_d12_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_12_1.txt";
    EXPECT_EQ(281691380235984, AOC2019::day12_2(pth.generic_string()));
}
TEST(A_2019_d13_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_13_1.txt";
    EXPECT_EQ(452, AOC2019::day13_1(pth.generic_string()));
}
TEST(A_2019_d13_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_13_1.txt";
    EXPECT_EQ(21415, AOC2019::day13_2(pth.generic_string()));
}
TEST(A_2019_d14_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_14_1.txt";
    EXPECT_EQ(899155, AOC2019::day14_1(pth.generic_string()));
}
TEST(A_2019_d14_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_14_1.txt";
    EXPECT_EQ(2390226, AOC2019::day14_2(pth.generic_string()));
}
TEST(A_2019_d15_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_15_1.txt";
    EXPECT_EQ(252, AOC2019::day15_1(pth.generic_string()));
}
TEST(A_2019_d15_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_15_1.txt";
    EXPECT_EQ(350, AOC2019::day15_2(pth.generic_string()));
}
TEST(A_2019_d16_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_16_1.txt";
    EXPECT_EQ(23135243, AOC2019::day16_1(pth.generic_string(),100));
}
TEST(A_2019_d16_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_16_1.txt";
    EXPECT_EQ(21130597, AOC2019::day16_2(pth.generic_string(),100));
}
TEST(A_2019_d17_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_17_1.txt";
    EXPECT_EQ(6212, AOC2019::day17_1(pth.generic_string()));
}
TEST(A_2019_d17_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_17_1.txt";
    EXPECT_EQ(1016741, AOC2019::day17_2(pth.generic_string()));
}
TEST(A_2019_d18_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_18_1.txt";
    EXPECT_EQ(3216, AOC2019::day18_1(pth.generic_string()));
}
TEST(A_2019_d18_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_18_1.txt";
    EXPECT_EQ(1538, AOC2019::day18_2(pth.generic_string()));
}
TEST(A_2019_d19_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_19_1.txt";
    EXPECT_EQ(197, AOC2019::day19_1(pth.generic_string()));
}
TEST(A_2019_d19_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_19_1.txt";
    EXPECT_EQ(9181022, AOC2019::day19_2(pth.generic_string()));
}
TEST(A_2019_d20_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_20_1.txt";
    EXPECT_EQ(458, AOC2019::day20_1(pth.generic_string()));
}
TEST(A_2019_d20_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_20_1.txt";
    EXPECT_EQ(5502, AOC2019::day20_2(pth.generic_string()));
}
TEST(A_2019_d21_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_21_1.txt";
    EXPECT_EQ(19356418, AOC2019::day21_1(pth.generic_string()));
}
TEST(A_2019_d21_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2019_21_1.txt";
    EXPECT_EQ(1146440619ll, AOC2019::day21_2(pth.generic_string()));
}