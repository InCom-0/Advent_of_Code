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