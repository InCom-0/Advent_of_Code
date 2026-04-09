#include <gtest/gtest.h>

#include <AOC_2025.h>
#include <incom_commons.h>

#define DATAFOLDER      "../../../data"
#define DATAFOLDER_2024 "../../../data/AOC_2025"


// TEST(A_2024_d1_1, A1) {
//     auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_1_1.txt";
//     EXPECT_EQ(2430334, AOC2025::day1_1(pth.generic_string()));
// }
// TEST(A_2024_d1_2, A1) {
//     auto pth = std::filesystem::path(DATAFOLDER_2024) / "2024_1_1.txt";
//     EXPECT_EQ(28786472, AOC2025::day1_2(pth.generic_string()));
// }

TEST(Incom_aoc_solver, finds_the_minimum_sum_nonNegativeInteger_solution_for_the_requested_system) {
	incom::aoc::solver underTest;

	underTest.build().input('a').input('c').input('d').equals(7);
	underTest.build().input('d').input('e').equals(5);
	underTest.build().input('a').input('b').input('d').input('e').equals(12);
	underTest.build().input('a').input('b').input('e').equals(7);
	underTest.build().input('a').input('c').input('e').equals(2);

	auto const bestSolution = underTest.solve();
	ASSERT_TRUE(bestSolution.has_value());
	EXPECT_EQ(2, bestSolution->value_of('a'));
	EXPECT_EQ(5, bestSolution->value_of('b'));
	EXPECT_EQ(0, bestSolution->value_of('c'));
	EXPECT_EQ(5, bestSolution->value_of('d'));
	EXPECT_EQ(0, bestSolution->value_of('e'));
	EXPECT_EQ(12, bestSolution->sum);

	auto const allSolutions = underTest.solve_all();
	ASSERT_EQ(3uz, allSolutions.size());
	EXPECT_EQ(12, allSolutions.front().sum);
	EXPECT_EQ(14, allSolutions.back().sum);
}

TEST(Incom_aoc_solver, finds_the_minimum_sum_nonNegativeInteger_solution) {
	incom::aoc::solver underTest;

	underTest.build().input('a').input('b').equals(5);
	underTest.build().input('b').input('c').equals(6);

	auto const bestSolution = underTest.solve();
	ASSERT_TRUE(bestSolution.has_value());
	EXPECT_EQ(0, bestSolution->value_of('a'));
	EXPECT_EQ(5, bestSolution->value_of('b'));
	EXPECT_EQ(1, bestSolution->value_of('c'));
	EXPECT_EQ(6, bestSolution->sum);

	auto const allSolutions = underTest.solve_all();
	ASSERT_EQ(6uz, allSolutions.size());
	EXPECT_EQ(6, allSolutions.front().sum);
	EXPECT_EQ(11, allSolutions.back().sum);
}

TEST(Incom_aoc_solver, propagates_single_remaining_distinct_variable) {
	incom::aoc::solver underTest;

	underTest.build().input('x').input('x').input('y').equals(8);
	underTest.build().input('y').equals(2);

	auto const bestSolution = underTest.solve();
	ASSERT_TRUE(bestSolution.has_value());
	EXPECT_EQ(3, bestSolution->value_of('x'));
	EXPECT_EQ(2, bestSolution->value_of('y'));
	EXPECT_EQ(5, bestSolution->sum);

	auto const allSolutions = underTest.solve_all();
	ASSERT_EQ(1uz, allSolutions.size());
	EXPECT_EQ(3, allSolutions.front().value_of('x'));
	EXPECT_EQ(2, allSolutions.front().value_of('y'));
}

TEST(Incom_aoc_solver, preserves_integrality_after_elimination) {
	incom::aoc::solver underTest;

	underTest.build().input('x').input('x').input('y').equals(5);

	auto const bestSolution = underTest.solve();
	ASSERT_TRUE(bestSolution.has_value());
	EXPECT_EQ(2, bestSolution->value_of('x'));
	EXPECT_EQ(1, bestSolution->value_of('y'));
	EXPECT_EQ(3, bestSolution->sum);

	auto const allSolutions = underTest.solve_all();
	ASSERT_EQ(3uz, allSolutions.size());
	EXPECT_EQ(2, allSolutions.at(0).value_of('x'));
	EXPECT_EQ(1, allSolutions.at(0).value_of('y'));
	EXPECT_EQ(0, allSolutions.at(2).value_of('x'));
	EXPECT_EQ(5, allSolutions.at(2).value_of('y'));
}
