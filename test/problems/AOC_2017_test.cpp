#include <gtest/gtest.h>

#include <filesystem>

#include <AOC_2017.h>
#include <AOC_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2017 "../../data/AOC_2017"


TEST(A_2017_d1_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_1_1.txt";
    EXPECT_EQ(1102, AOC2017::day1_1(pth.generic_string()));
}
TEST(A_2017_d1_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_1_1.txt";
    EXPECT_EQ(1076, AOC2017::day1_2(pth.generic_string()));
}

TEST(A_2017_d2_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_2_1.txt";
    EXPECT_EQ(58975, AOC2017::day2_1(pth.generic_string()));
}
TEST(A_2017_d2_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_2_1.txt";
    EXPECT_EQ(308, AOC2017::day2_2(pth.generic_string()));
}

TEST(A_2017_d3_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_3_1.txt";
    EXPECT_EQ(419, AOC2017::day3_1(pth.generic_string()));
}
TEST(A_2017_d3_3, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_3_1.txt";
    EXPECT_EQ(295229, AOC2017::day3_3(pth.generic_string()));
}

TEST(A_2017_d4_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_4_1.txt";
    EXPECT_EQ(337, AOC2017::day4_1(pth.generic_string()));
}
TEST(A_2017_d4_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_4_1.txt";
    EXPECT_EQ(231, AOC2017::day4_2(pth.generic_string()));
}
TEST(A_2017_d5_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_5_1.txt";
    EXPECT_EQ(336905, AOC2017::day5_1(pth.generic_string()));
}
TEST(A_2017_d5_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_5_1.txt";
    EXPECT_EQ(21985262, AOC2017::day5_2(pth.generic_string()));
}
TEST(A_2017_d5_2, A3) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_5_1.txt";
    EXPECT_EQ(21985262, AOC2017::day5_3(pth.generic_string()));
}
TEST(A_2017_d6_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_6_1.txt";
    EXPECT_EQ(7864, AOC2017::day6_1(pth.generic_string()));
}
TEST(A_2017_d6_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_6_1.txt";
    EXPECT_EQ(1695, AOC2017::day6_2(pth.generic_string()));
}
TEST(A_2017_d7_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_7_1.txt";
    EXPECT_EQ("vvsvez", AOC2017::day7_1(pth.generic_string()));
}
TEST(A_2017_d7_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_7_1.txt";
    EXPECT_EQ(362, AOC2017::day7_2(pth.generic_string()));
}
TEST(A_2017_d8_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_8_1.txt";
    EXPECT_EQ(4448, AOC2017::day8_1(pth.generic_string()));
}
TEST(A_2017_d8_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_8_1.txt";
    EXPECT_EQ(6582, AOC2017::day8_2(pth.generic_string()));
}
TEST(A_2017_d9_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_9_1.txt";
    EXPECT_EQ(11898, AOC2017::day9_1(pth.generic_string()));
}
TEST(A_2017_d9_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_9_1.txt";
    EXPECT_EQ(5601, AOC2017::day9_2(pth.generic_string()));
}
TEST(A_2017_d10_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_10_1.txt";
    EXPECT_EQ(4114, AOC2017::day10_1(pth.generic_string()));
}
TEST(A_2017_d10_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_10_1.txt";
    EXPECT_EQ("2f8c3d2100fdd57cec130d928b0fd2dd", AOC2017::day10_2(pth.generic_string()));
}
TEST(A_2017_d11_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_11_1.txt";
    EXPECT_EQ(722, AOC2017::day11_1(pth.generic_string()));
}
TEST(A_2017_d11_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_11_1.txt";
    EXPECT_EQ(1551, AOC2017::day11_2(pth.generic_string()));
}
TEST(A_2017_d12_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_12_1.txt";
    EXPECT_EQ(113, AOC2017::day12_1(pth.generic_string()));
}
TEST(A_2017_d12_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_12_1.txt";
    EXPECT_EQ(202, AOC2017::day12_2(pth.generic_string()));
}
TEST(A_2017_d13_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_13_1.txt";
    EXPECT_EQ(1904, AOC2017::day13_1(pth.generic_string()));
}
TEST(A_2017_d13_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_13_1.txt";
    EXPECT_EQ(3833504, AOC2017::day13_2(pth.generic_string()));
}
TEST(A_2017_d14_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_14_1.txt";
    EXPECT_EQ(8194, AOC2017::day14_1(pth.generic_string()));
}
TEST(A_2017_d14_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_14_1.txt";
    EXPECT_EQ(1141, AOC2017::day14_2(pth.generic_string()));
}
TEST(A_2017_d15_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_15_1.txt";
    EXPECT_EQ(567, AOC2017::day15_1(pth.generic_string()));
}
TEST(A_2017_d15_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_15_1.txt";
    EXPECT_EQ(323, AOC2017::day15_2(pth.generic_string()));
}
TEST(A_2017_d16_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_16_1.txt";
    EXPECT_EQ("lgpkniodmjacfbeh", AOC2017::day16_1(pth.generic_string()));
}
TEST(DISABLED_A_2017_d16_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_16_1.txt";
    EXPECT_EQ("hklecbpnjigoafmd", AOC2017::day16_2(pth.generic_string()));
}
TEST(A_2017_d16_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_16_1.txt";
    EXPECT_EQ("hklecbpnjigoafmd", AOC2017::day16_3(pth.generic_string(), 1'000'000'000));
}
TEST(A_2017_d17_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_17_1.txt";
    EXPECT_EQ(996, AOC2017::day17_1(pth.generic_string()));
}
TEST(A_2017_d17_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_17_1.txt";
    EXPECT_EQ(1898341, AOC2017::day17_2(pth.generic_string()));
}
TEST(A_2017_d18_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_18_1.txt";
    EXPECT_EQ(9423, AOC2017::day18_1(pth.generic_string()));
}
TEST(A_2017_d18_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_18_1.txt";
    EXPECT_EQ(7620, AOC2017::day18_2(pth.generic_string(), 2));
}
TEST(A_2017_d19_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_19_1.txt";
    EXPECT_EQ("GPALMJSOY", AOC2017::day19_1(pth.generic_string()));
}
TEST(A_2017_d19_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_19_1.txt";
    EXPECT_EQ(16204, AOC2017::day19_2(pth.generic_string()));
}
TEST(A_2017_d20_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_20_1.txt";
    EXPECT_EQ(119, AOC2017::day20_1(pth.generic_string()));
}
TEST(A_2017_d20_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_20_1.txt";
    EXPECT_EQ(471, AOC2017::day20_2(pth.generic_string()));
}
TEST(A_2017_d21_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_21_1.txt";
    EXPECT_EQ(190, AOC2017::day21_1(pth.generic_string()));
}
TEST(A_2017_d21_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_21_1.txt";
    EXPECT_EQ(2335049, AOC2017::day21_2(pth.generic_string()));
}
TEST(A_2017_d22_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_22_1.txt";
    EXPECT_EQ(5406, AOC2017::day22_1(pth.generic_string(), 10'000));
}
TEST(DISABLED_A_2017_d22_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_22_1.txt";
    EXPECT_EQ(2511640, AOC2017::day22_2(pth.generic_string(), 10'000'000));
}
TEST(A_2017_d22_2, A2) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_22_1.txt";
    EXPECT_EQ(2511640, AOC2017::day22_3(pth.generic_string(), 10'000'000));
}
TEST(A_2017_d23_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_23_1.txt";
    EXPECT_EQ(3969, AOC2017::day23_1(pth.generic_string()));
}
TEST(A_2017_d23_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_23_3.txt";
    EXPECT_EQ(917, AOC2017::day23_2(pth.generic_string()));
}
TEST(A_2017_d24_1, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_24_1.txt";
    EXPECT_EQ(1859, AOC2017::day24_1(pth.generic_string()));
}
TEST(A_2017_d24_2, A1) {
    auto pth = std::filesystem::path(DATAFOLDER_2017) / "2017_24_1.txt";
    EXPECT_EQ(1799, AOC2017::day24_2(pth.generic_string()));
}