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