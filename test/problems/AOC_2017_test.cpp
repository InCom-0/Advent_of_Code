#include <gtest/gtest.h>
#include <vector>

#include <filesystem>

#include <AOC_2015.h>
#include <AOC_2016.h>
#include <AOC_2017.h>
#include <AOC_commons.h>

#define DATAFOLDER "../../data"
#define DATAFOLDER_2017 "../../data/AOC_2017"



TEST(A_2017_d1_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_1_1.txt";
    EXPECT_EQ(1102,AOC2017::day1_1(pth.generic_string()));
}
TEST(A_2017_d1_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_1_1.txt";
    EXPECT_EQ(1076,AOC2017::day1_2(pth.generic_string()));
}

TEST(A_2017_d2_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_2_1.txt";
    EXPECT_EQ(58975,AOC2017::day2_1(pth.generic_string()));
}
TEST(A_2017_d2_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_2_1.txt";
    EXPECT_EQ(308,AOC2017::day2_2(pth.generic_string()));
}

TEST(A_2017_d3_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_3_1.txt";
    EXPECT_EQ(419,AOC2017::day3_1(pth.generic_string()));
}
TEST(A_2017_d3_3, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_3_1.txt";
    EXPECT_EQ(295229,AOC2017::day3_3(pth.generic_string()));
}

TEST(A_2017_d4_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_4_1.txt";
    EXPECT_EQ(337,AOC2017::day4_1(pth.generic_string()));
}
TEST(A_2017_d4_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_4_1.txt";
    EXPECT_EQ(231,AOC2017::day4_2(pth.generic_string()));
}
TEST(A_2017_d5_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_5_1.txt";
    EXPECT_EQ(336905,AOC2017::day5_1(pth.generic_string()));
}
TEST(A_2017_d5_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_5_1.txt";
    EXPECT_EQ(21985262,AOC2017::day5_2(pth.generic_string()));
}
TEST(A_2017_d5_2, A3) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_5_1.txt";
    EXPECT_EQ(21985262,AOC2017::day5_3(pth.generic_string()));
}
TEST(A_2017_d6_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_6_1.txt";
    EXPECT_EQ(7864,AOC2017::day6_1(pth.generic_string()));
}
TEST(A_2017_d6_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_6_1.txt";
    EXPECT_EQ(1695,AOC2017::day6_2(pth.generic_string()));
}
TEST(A_2017_d7_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_7_1.txt";
    EXPECT_EQ("vvsvez",AOC2017::day7_1(pth.generic_string()));
}
TEST(A_2017_d7_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017)/ "2017_7_1.txt";
    EXPECT_EQ(362,AOC2017::day7_2(pth.generic_string()));
}