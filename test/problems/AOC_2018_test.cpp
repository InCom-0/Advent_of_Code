#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2018.h>
#include <incom_commons.h>

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
TEST(A_2018_d13_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_13_1.txt";
    EXPECT_EQ("116,10", AOC2018::day13_1(pth.generic_string()));
}
TEST(A_2018_d13_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_13_1.txt";
    EXPECT_EQ("116,25", AOC2018::day13_2(pth.generic_string()));
}
TEST(A_2018_d14_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_14_1.txt";
    EXPECT_EQ(3656126723, AOC2018::day14_1(pth.generic_string()));
}
TEST(A_2018_d14_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_14_1.txt";
    EXPECT_EQ(20333868, AOC2018::day14_2(pth.generic_string()));
}
TEST(DISABLED_A_2018_d14_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_14_1.txt";
    EXPECT_EQ(20333868, AOC2018::day14_3(pth.generic_string()));
}
TEST(A_2018_d14_2, A3) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_14_1.txt";
    EXPECT_EQ(20333868, AOC2018::day14_4(pth.generic_string()));
}
TEST(A_2018_d14_2, A4) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_14_1.txt";
    EXPECT_EQ(20333868, AOC2018::day14_5(pth.generic_string()));
}
TEST(A_2018_d15_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_15_1.txt";
    EXPECT_EQ(207542, AOC2018::day15_1(pth.generic_string()));
}
TEST(A_2018_d15_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_15_1.txt";
    EXPECT_EQ(64688, AOC2018::day15_2(pth.generic_string()));
}
TEST(A_2018_d16_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_16_1.txt";
    EXPECT_EQ(544, AOC2018::day16_1(pth.generic_string(), 3));
}
TEST(A_2018_d16_2, A1) {
    auto pth   = std::filesystem::path(DATAFOLDER_2018) / "2018_16_1.txt";
    auto pth_2 = std::filesystem::path(DATAFOLDER_2018) / "2018_16_2.txt";
    EXPECT_EQ(600, AOC2018::day16_2(pth.generic_string(), pth_2.generic_string()));
}
TEST(A_2018_d17_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_17_1.txt";
    EXPECT_EQ(33242, AOC2018::day17_1(pth.generic_string()));
}
TEST(A_2018_d17_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_17_1.txt";
    EXPECT_EQ(27256, AOC2018::day17_2(pth.generic_string()));
}
TEST(A_2018_d18_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_18_1.txt";
    EXPECT_EQ(558960, AOC2018::day18_1(pth.generic_string()));
}
TEST(A_2018_d18_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_18_1.txt";
    EXPECT_EQ(207900, AOC2018::day18_2(pth.generic_string()));
}
TEST(A_2018_d19_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_19_1.txt";
    EXPECT_EQ(1500, AOC2018::day19_1(pth.generic_string(), {0, 0, 0, 0, 0, 0}));
}
TEST(A_2018_d19_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_19_1.txt";
    EXPECT_EQ(18869760, AOC2018::day19_2(pth.generic_string(), {1, 0, 0, 0, 0, 0}));
}
TEST(A_2018_d20_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_20_1.txt";
    EXPECT_EQ(3930, AOC2018::day20_1(pth.generic_string()).first);
}
TEST(A_2018_d20_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_20_1.txt";
    EXPECT_EQ(8240, AOC2018::day20_1(pth.generic_string()).second);
}

TEST(A_2018_d21_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_21_1.txt";
    EXPECT_EQ(6132825, AOC2018::day21_1(pth.generic_string()));
}
TEST(DISABLED_A_2018_d21_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_21_1.txt";
    EXPECT_EQ(8307757, AOC2018::day21_2(pth.generic_string()));
}

TEST(A_2018_d22_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_22_1.txt";
    EXPECT_EQ(8735, AOC2018::day22_1(pth.generic_string()));
}
TEST(A_2018_d22_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_22_1.txt";
    EXPECT_EQ(984, AOC2018::day22_2(pth.generic_string()));
}

TEST(A_2018_d23_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_23_1.txt";
    EXPECT_EQ(640, AOC2018::day23_1(pth.generic_string()));
}
TEST(A_2018_d23_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_23_1.txt";
    EXPECT_EQ(113066145LL, AOC2018::day23_2(pth.generic_string()));
}
TEST(A_2018_d24_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_24_1.txt";
    EXPECT_EQ(28976, AOC2018::day24_1(pth.generic_string()));
}
TEST(A_2018_d24_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_24_1.txt";
    EXPECT_EQ(3534, AOC2018::day24_2(pth.generic_string()));
}
TEST(A_2018_d25_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2018) / "2018_25_1.txt";
    EXPECT_EQ(305, AOC2018::day25_1(pth.generic_string()));
}