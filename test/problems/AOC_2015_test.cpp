#include <filesystem>
#include <gtest/gtest.h>

#include <AOC_2015.h>
#include <AOC_commons.h>

#define DATAFOLDER      "../../data"
#define DATAFOLDER_2015 "../../data/AOC_2015"


TEST(A_2015_d1_1, A1) {

    std::ifstream iStream;
    iStream.clear();
    iStream.open(std::filesystem::path(DATAFOLDER_2015) / "2015_1_1.txt");
    std::string oneStr;

    std::getline(iStream, oneStr);

    EXPECT_EQ(74, AOC2015::day1_1(oneStr));
}
TEST(A_2015_d1_2, A1) {

    std::ifstream iStream;
    iStream.clear();
    iStream.open(std::filesystem::path(DATAFOLDER_2015) / "2015_1_1.txt");
    std::string oneStr;

    std::getline(iStream, oneStr);

    EXPECT_EQ(1795, AOC2015::day1_2(oneStr));
}

TEST(A_2015_d2_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_2_1.txt";

    EXPECT_EQ(1588178, AOC2015::day2_1(pth.generic_string()));
}
TEST(A_2015_d2_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_2_1.txt";

    EXPECT_EQ(3783758, AOC2015::day2_2(pth.generic_string()));
}

TEST(A_2015_d3_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_3_1.txt";

    EXPECT_EQ(2565, AOC2015::day3_1(pth.generic_string()));
}
TEST(A_2015_d3_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_3_1.txt";

    EXPECT_EQ(2639, AOC2015::day3_2(pth.generic_string()));
}

TEST(DISABLED_A_2015_d4_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_4_1.txt";

    EXPECT_EQ(1, 0);
}
TEST(DISABLED_A_2015_d4_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_4_1.txt";

    EXPECT_EQ(1, 0);
}

TEST(A_2015_d5_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_5_1.txt";

    EXPECT_EQ(258, AOC2015::day5_1(pth.generic_string()));
}
TEST(A_2015_d5_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_5_1.txt";

    EXPECT_EQ(53, AOC2015::day5_2(pth.generic_string()));
}

TEST(A_2015_d6_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_6_1.txt";

    EXPECT_EQ(377891, AOC2015::day6_1(pth.generic_string()));
}
TEST(A_2015_d6_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_6_1.txt";

    EXPECT_EQ(14110788, AOC2015::day6_2(pth.generic_string()));
}

TEST(DISABLED_A_2015_d7_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_7_1.txt";

    EXPECT_EQ(1, 0);
}
TEST(DISABLED_A_2015_d7_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_7_1.txt";

    EXPECT_EQ(1, 0);
}

TEST(A_2015_d8_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_8_1.txt";

    EXPECT_EQ(1350, AOC2015::day8_1(pth.generic_string()));
}
TEST(A_2015_d8_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_8_1.txt";

    EXPECT_EQ(2085, AOC2015::day8_2(pth.generic_string()));
}

TEST(A_2015_d9_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_9_1.txt";

    EXPECT_EQ(207, AOC2015::day9_1(pth.generic_string()));
}
TEST(A_2015_d9_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_9_1.txt";

    EXPECT_EQ(804, AOC2015::day9_2(pth.generic_string()));
}

TEST(A_2015_d10_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_10_1.txt";

    EXPECT_EQ(329356, AOC2015::day10_1(pth.generic_string(), 40));
}
TEST(A_2015_d10_1, A2) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_10_1.txt";

    EXPECT_EQ(329356, AOC2015::day10_2(pth.generic_string(), 40));
}
TEST(DISABLED_A_2015_d10_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_10_1.txt";

    EXPECT_EQ(4666278, AOC2015::day10_1(pth.generic_string(), 50));
}
TEST(DISABLED_A_2015_d10_2, A2) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_10_1.txt";

    EXPECT_EQ(4666278, AOC2015::day10_2(pth.generic_string(), 50));
}

TEST(A_2015_d12_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_12_1.json";

    EXPECT_EQ(119433, AOC2015::day12_1(pth.generic_string()));
}
TEST(A_2015_d12_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_12_1.json";

    EXPECT_EQ(68466, AOC2015::day12_2(pth.generic_string()));
}

TEST(A_2015_d13_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_13_1.txt";

    EXPECT_EQ(733, AOC2015::day13_1(pth.generic_string()));
}
TEST(A_2015_d13_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_13_2.txt";

    EXPECT_EQ(725, AOC2015::day13_1(pth.generic_string()));
}

TEST(A_2015_d14_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_14_1.txt";

    EXPECT_EQ(2640, AOC2015::day14_1(pth.generic_string(), 2503));
}
TEST(A_2015_d14_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_14_1.txt";

    EXPECT_EQ(1102, AOC2015::day14_2(pth.generic_string(), 2503));
}

TEST(DISABLED_A_2015_d15_1, A1) {

    EXPECT_EQ(1, 0);
}
TEST(DISABLED_A_2015_d15_2, A1) {

    EXPECT_EQ(1, 0);
}

TEST(A_2015_d16_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_16_1.txt";

    EXPECT_EQ(213, AOC2015::day16_1(pth.generic_string()));
}
TEST(A_2015_d16_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_16_1.txt";

    EXPECT_EQ(323, AOC2015::day16_2(pth.generic_string()));
}

TEST(A_2015_d17_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_17_1.txt";

    EXPECT_EQ(654, AOC2015::day17_1(pth.generic_string(), 150));
}
TEST(A_2015_d17_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_17_1.txt";

    EXPECT_EQ(57, AOC2015::day17_2(pth.generic_string(), 150));
}

TEST(A_2015_d18_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_18_1.txt";

    EXPECT_EQ(768, AOC2015::day18_1(pth.generic_string(), 100, 100));
}
TEST(A_2015_d18_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_18_1.txt";

    EXPECT_EQ(781, AOC2015::day18_2(pth.generic_string(), 100, 100));
}

TEST(A_2015_d19_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_19_1.txt";

    EXPECT_EQ(509, AOC2015::day19_1(
                       pth.generic_string(),
                       "CRnCaSiRnBSiRnFArTiBPTiTiBFArPBCaSiThSiRnTiBPBPMgArCaSiRnTiMgArCaSiThCaSiRnFArRnSiRnFArTiTiBFAr"
                       "CaCaSiRnSiThCaCaSiRnMgArFYSiRnFYCaFArSiThCaSiThPBPTiMgArCaPRnSiAlArPBCaCaSiRnFYSiThCaRnFArArCaC"
                       "aSiRnPBSiRnFArMgYCaCaCaCaSiThCaCaSiAlArCaCaSiRnPBSiAlArBCaCaCaCaSiThCaPBSiThPBPBCaSiRnFYFArSiTh"
                       "CaSiRnFArBCaCaSiRnFYFArSiThCaPBSiThCaSiRnPMgArRnFArPTiBCaPRnFArCaCaCaCaSiRnCaCaSiRnFYFArFArBCaS"
                       "iThFArThSiThSiRnTiRnPMgArFArCaSiThCaPBCaSiRnBFArCaCaPRnCaCaPMgArSiRnFYFArCaSiThRnPBPMgAr"));
}
TEST(A_2015_d19_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_19_1.txt";

    EXPECT_EQ(195, AOC2015::day19_3(
                       pth.generic_string(),
                       "CRnCaSiRnBSiRnFArTiBPTiTiBFArPBCaSiThSiRnTiBPBPMgArCaSiRnTiMgArCaSiThCaSiRnFArRnSiRnFArTiTiBFAr"
                       "CaCaSiRnSiThCaCaSiRnMgArFYSiRnFYCaFArSiThCaSiThPBPTiMgArCaPRnSiAlArPBCaCaSiRnFYSiThCaRnFArArCaC"
                       "aSiRnPBSiRnFArMgYCaCaCaCaSiThCaCaSiAlArCaCaSiRnPBSiAlArBCaCaCaCaSiThCaPBSiThPBPBCaSiRnFYFArSiTh"
                       "CaSiRnFArBCaCaSiRnFYFArSiThCaPBSiThCaSiRnPMgArRnFArPTiBCaPRnFArCaCaCaCaSiRnCaCaSiRnFYFArFArBCaS"
                       "iThFArThSiThSiRnTiRnPMgArFArCaSiThCaPBCaSiRnBFArCaCaPRnCaCaPMgArSiRnFYFArCaSiThRnPBPMgAr"));
}

TEST(A_2015_d20_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_20_1.txt";

    EXPECT_EQ(665280, AOC2015::day20_1(pth.generic_string()));
}
TEST(A_2015_d20_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_20_1.txt";

    EXPECT_EQ(705600, AOC2015::day20_2(pth.generic_string()));
}

TEST(A_2015_d21_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_21_1.txt";

    EXPECT_EQ(111, AOC2015::day21_1(109, 8, 2, 100, pth.generic_string()));
}
TEST(A_2015_d21_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_21_1.txt";

    EXPECT_EQ(188, AOC2015::day21_2(109, 8, 2, 100, pth.generic_string()));
}

TEST(A_2015_d22_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_22_1.txt";

    EXPECT_EQ(1269, AOC2015::day22_1(59, 9, 50, 500));
}
TEST(A_2015_d22_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_22_1.txt";

    EXPECT_EQ(1309, AOC2015::day22_2(59, 9, 50, 500));
}

TEST(A_2015_d23_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_23_1.txt";

    EXPECT_EQ(255, AOC2015::day23_1(pth.generic_string(), 0, 0));
}
TEST(A_2015_d23_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_23_1.txt";

    EXPECT_EQ(334, AOC2015::day23_1(pth.generic_string(), 1, 0));
}

TEST(A_2015_d24_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_24_1.txt";

    EXPECT_EQ(11266889531, AOC2015::day24_1(pth.generic_string(), 3));
}
TEST(A_2015_d24_2, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_24_1.txt";

    EXPECT_EQ(77387711, AOC2015::day24_1(pth.generic_string(), 4));
}

TEST(A_2015_d25_1, A1) {

    auto pth = std::filesystem::path(DATAFOLDER_2015) / "2015_25_1.txt";

    EXPECT_EQ(2650453, AOC2015::day25_1(20151125, 252533, 33554393, 2978, 3083));
}