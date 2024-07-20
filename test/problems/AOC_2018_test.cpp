#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2018.h>
#include <AOC_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2018 "../../data/AOC_2018"

TEST(A_2018_d1_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_1_1.txt";
    EXPECT_EQ(540, AOC2018::day1_1(pth.generic_string()));
}
TEST(A_2018_d1_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_1_1.txt";
    EXPECT_EQ(73056, AOC2018::day1_2(pth.generic_string()));
}
TEST(A_2018_d2_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_2_1.txt";
    EXPECT_EQ(5000, AOC2018::day2_1(pth.generic_string()));
}
TEST(A_2018_d2_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_2_1.txt";
    EXPECT_EQ("ymdrchgpvwfloluktajxijsqb", AOC2018::day2_2(pth.generic_string()));
}
TEST(A_2018_d3_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_3_1.txt";
    EXPECT_EQ(104439, AOC2018::day3_1(pth.generic_string()));
}
TEST(A_2018_d3_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_3_1.txt";
    EXPECT_EQ(701, AOC2018::day3_2(pth.generic_string()));
}
TEST(A_2018_d4_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_4_1.txt";
    EXPECT_EQ(39584, AOC2018::day4_1(pth.generic_string()));
}
TEST(A_2018_d4_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_4_1.txt";
    EXPECT_EQ(55053, AOC2018::day4_2(pth.generic_string()));
}
TEST(A_2018_d5_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_5_1.txt";
    EXPECT_EQ(10886, AOC2018::day5_1(pth.generic_string()));
}
TEST(A_2018_d5_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_5_1.txt";
    EXPECT_EQ(4684, AOC2018::day5_2(pth.generic_string()));
}
TEST(A_2018_d6_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_6_1.txt";
    EXPECT_EQ(4829, AOC2018::day6_1(pth.generic_string()));
}
TEST(A_2018_d6_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_6_1.txt";
    EXPECT_EQ(46966, AOC2018::day6_2(pth.generic_string(), 10'000));
}
TEST(A_2018_d7_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_7_1.txt";
    EXPECT_EQ("BFKEGNOVATIHXYZRMCJDLSUPWQ", AOC2018::day7_1(pth.generic_string()));
}
TEST(A_2018_d7_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_7_1.txt";
    EXPECT_EQ(1020, AOC2018::day7_2(pth.generic_string(), 5));
}
TEST(A_2018_d8_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_8_1.txt";
    EXPECT_EQ(44838, AOC2018::day8_1(pth.generic_string()));
}
TEST(A_2018_d8_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_8_1.txt";
    EXPECT_EQ(22198, AOC2018::day8_2(pth.generic_string()));
}
TEST(A_2018_d9_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_9_1.txt";
    EXPECT_EQ(367802, AOC2018::day9_1(pth.generic_string()));
}
TEST(A_2018_d9_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_9_2.txt";
    EXPECT_EQ(2996043280, AOC2018::day9_1(pth.generic_string()));
}
TEST(A_2018_d10_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_10_1.txt";
    EXPECT_EQ(10356, AOC2018::day10_1(pth.generic_string()));
}
TEST(A_2018_d11_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_11_1.txt";
    EXPECT_EQ("235,16", AOC2018::day11_1(pth.generic_string()));
}
TEST(DISABLED_A_2018_d11_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_11_1.txt";
    EXPECT_EQ("236,227,14", AOC2018::day11_2(pth.generic_string()));
}
TEST(A_2018_d11_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_11_1.txt";
    EXPECT_EQ("236,227,14", AOC2018::day11_3(pth.generic_string()));
}
TEST(A_2018_d12_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_12_1.txt";
    EXPECT_EQ(4110, AOC2018::day12_1(pth.generic_string(), 20));
}
TEST(A_2018_d12_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_12_1.txt";
    EXPECT_EQ(2650000000466, AOC2018::day12_2(pth.generic_string(), 50'000'000'000));
}