#include <ankerl/unordered_dense.h>
#include <cstddef>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>


namespace AOC2025 {
size_t
day7_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    char const split      = '^';
    char const split_used = 'v';


    auto recSolver = [&](this auto const &self, int row, int col) -> int {
        input[row][col] = split_used;
        --row;

        size_t accu_recursed = 0uz;
        bool   enabled       = false;
        while (row >= 0) {
            if (input[row][col] == 'S') {
                enabled = true;
                break;
            }
            else if (input[row][col] != '.') { break; }

            if (input[row][col - 1] == split) {
                accu_recursed += self(row, col - 1);
                enabled       |= accu_recursed;
            }
            else if (input[row][col - 1] == split_used) { enabled = true; }


            if (input[row][col + 1] == split) {
                accu_recursed += self(row, col + 1);
                enabled       |= accu_recursed;
            }
            else if (input[row][col + 1] == split_used) { enabled = true; }

            row--;
        }

        return accu_recursed + enabled;
    };

    size_t accu = 0uz;
    for (int row = input.size() - 1; row >= 0; --row) {
        for (int col = input.at(row).size() - 1; col >= 0; --col) {
            if (input[row][col] == split) { accu += recSolver(row, col); }
        }
    }

    return accu;
}
size_t
day7_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(.+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processFile(dataFile, d_ctre).front();

    char const split      = '^';
    std::vector memo{input.size(), std::vector(input.front().size(), std::optional<size_t>{})};

    auto recSolver = [&](this auto const &self, int row, int col) -> size_t {
        if (memo[row][col]) { return memo[row][col].value(); }

        int oldRow = row;
        row--;

        size_t accu_recursed = 0uz;
        while (row >= 0) {
            if (input[row][col] == 'S') {
                accu_recursed = 1uz;
                break;
            }
            else if (input[row][col] != '.') { break; }

            if (col > 0 && input[row][col - 1] == split) { accu_recursed += self(row, col - 1); }
            if ((col < (memo.front().size() - 1)) && input[row][col + 1] == split) {
                accu_recursed += self(row, col + 1);
            }

            row--;
        }

        memo[oldRow][col] = accu_recursed;
        return accu_recursed;
    };

    size_t accu    = 0uz;
    int    lastRow = input.size() - 1;

    for (int col = input.at(lastRow).size() - 1; col >= 0; --col) { accu += recSolver(lastRow, col); }

    return accu;
}
} // namespace AOC2025