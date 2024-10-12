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