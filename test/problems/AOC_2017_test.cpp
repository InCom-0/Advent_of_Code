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