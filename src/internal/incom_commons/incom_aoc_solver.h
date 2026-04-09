#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>


namespace incom {
namespace aoc {

/*
Elimination-first solver for small systems of sum equations over non-negative integers.

Model:
- Each equation is a sum of named variables with implicit coefficient 1.
- Variables are restricted to non-negative integer values.
- Repeating the same variable inside one equation is allowed and acts as a larger coefficient.

Usage example:

    incom::aoc::solver s;
    s.build().input('a').input('c').input('d').equals(7);
    s.build().input('d').input('e').equals(5);

    auto best = s.solve();      // minimum-sum solution, if one exists
    auto all  = s.solve_all();  // every solution, ordered by total sum

The implementation first reduces the equation system into pivot variables expressed as affine functions of
free variables. Search then happens only over the free variables rather than over every original variable.
This is much more effective for underdetermined systems such as 13 variables constrained by 10 equations.
*/
class solver {
private:
    struct rational {
        long long numerator   = 0;
        long long denominator = 1;

        rational() = default;

        rational(long long numeratorIn, long long denominatorIn = 1)
            : numerator(numeratorIn), denominator(denominatorIn) {
            assert(denominator != 0);
            normalize();
        }

        [[nodiscard]] bool
        is_zero() const {
            return numerator == 0;
        }

        [[nodiscard]] bool
        is_integer() const {
            return denominator == 1;
        }

        [[nodiscard]] long long
        as_integer() const {
            assert(is_integer());
            return numerator;
        }

        void
        normalize() {
            if (denominator < 0) {
                numerator   = -numerator;
                denominator = -denominator;
            }

            auto const divisor = std::gcd(numerator, denominator);
            if (divisor != 0) {
                numerator /= divisor;
                denominator /= divisor;
            }
        }
    };

    friend rational
    operator+(rational lhs, rational const &rhs) {
        lhs.numerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator-(rational lhs, rational const &rhs) {
        lhs.numerator = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator-(rational value) {
        return rational(-value.numerator, value.denominator);
    }

    friend rational
    operator*(rational lhs, rational const &rhs) {
        lhs.numerator *= rhs.numerator;
        lhs.denominator *= rhs.denominator;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator*(rational lhs, long long rhs) {
        lhs.numerator *= rhs;
        lhs.normalize();
        return lhs;
    }

    friend rational
    operator*(long long lhs, rational rhs) {
        rhs.numerator *= lhs;
        rhs.normalize();
        return rhs;
    }

    friend rational
    operator/(rational lhs, rational const &rhs) {
        assert(rhs.numerator != 0);
        lhs.numerator *= rhs.denominator;
        lhs.denominator *= rhs.numerator;
        lhs.normalize();
        return lhs;
    }

    friend bool
    operator==(rational const &lhs, rational const &rhs) {
        return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
    }

    friend bool
    operator<(rational const &lhs, rational const &rhs) {
        return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
    }

    friend bool
    operator>(rational const &lhs, rational const &rhs) {
        return rhs < lhs;
    }

    friend bool
    operator<=(rational const &lhs, rational const &rhs) {
        return not(rhs < lhs);
    }

    friend bool
    operator>=(rational const &lhs, rational const &rhs) {
        return not(lhs < rhs);
    }

    // One linear equation of the form x1 + x2 + ... + xn = rhs.
    struct equation {
        std::vector<size_t> variableIds;
        int                 rhs      = 0;
        bool                isClosed = false;
    };

    struct affine_expression {
        rational              constant{};
        std::vector<rational> freeCoefficients;
    };

    // Preprocessed information extracted from elimination.
    struct prepared_problem {
        std::vector<size_t> variableOrder;
        std::vector<int>    staticUpperBounds;
        std::vector<int>    freeIndexByVariable;
        std::vector<size_t> pivotVariables;
        std::vector<affine_expression> variableExpressions;
        rational                    objectiveConstant{};
        std::vector<rational>       objectiveWeights;
        bool                        isValid = true;
    };

public:
    // Concrete assignment returned by solve()/solve_all().
    struct solution {
        std::map<std::string, int> values;
        int                        sum = 0;

        // Convenience lookup by variable name. Asserts if the variable was never part of the system.
        [[nodiscard]] int
        value_of(std::string_view variableName) const {
            auto found = values.find(std::string(variableName));
            assert(found != values.end());
            return found->second;
        }

        // Convenience overload for one-character variable names.
        [[nodiscard]] int
        value_of(char variableName) const {
            return value_of(std::string_view(&variableName, 1));
        }
    };

    // Fluent builder used to define one equation term-by-term.
    class equation_builder {
    public:
        equation_builder(solver &owner, size_t equationId) : m_owner(owner), m_equationId(equationId) {}

        // Adds one occurrence of a variable to the current equation.
        // Calling input("x") twice means 2*x in the final equation.
        equation_builder &
        input(std::string_view variableName) {
            assert(not variableName.empty());
            auto &equation = m_owner.m_equations.at(m_equationId);
            assert(not equation.isClosed);
            equation.variableIds.push_back(m_owner.get_orCreateVariableId(variableName));
            return *this;
        }

        // Convenience overload for one-character variable names.
        equation_builder &
        input(char variableName) {
            return input(std::string_view(&variableName, 1));
        }

        // Finalizes the current equation as sum(inputs...) = rhs.
        solver &
        equals(int rhs) {
            auto &equation = m_owner.m_equations.at(m_equationId);
            assert(not equation.isClosed);
            assert(rhs >= 0);
            equation.rhs      = rhs;
            equation.isClosed = true;
            return m_owner;
        }

    private:
        solver &m_owner;
        size_t  m_equationId;
    };

    // Starts definition of a new equation.
    [[nodiscard]] equation_builder
    build() {
        m_equations.push_back(equation{});
        return equation_builder(*this, m_equations.size() - 1);
    }

    // Returns one solution with minimum total variable sum, if the system is solvable.
    [[nodiscard]] std::optional<solution>
    solve() const {
        return solve_minSum();
    }

    // Same as solve(); kept as the explicit name for callers that want to emphasize the objective.
    [[nodiscard]] std::optional<solution>
    solve_minSum() const {
        auto prepared = prepare_problem();
        if (not prepared.isValid) { return std::nullopt; }

        std::vector<int>        assignedFreeValues(prepared.variableOrder.size(), 0);
        std::vector<bool>       assignedFreeMask(prepared.variableOrder.size(), false);
        std::optional<solution> bestSolution;
        std::optional<int>      bestSum;

        search_minSum(prepared, assignedFreeValues, assignedFreeMask, 0, bestSolution, bestSum);
        return bestSolution;
    }

    // Enumerates every non-negative integer solution and returns them ordered by total sum.
    [[nodiscard]] std::vector<solution>
    solve_all() const {
        auto prepared = prepare_problem();
        if (not prepared.isValid) { return {}; }

        std::vector<int>      assignedFreeValues(prepared.variableOrder.size(), 0);
        std::vector<bool>     assignedFreeMask(prepared.variableOrder.size(), false);
        std::vector<solution> allSolutions;

        search_all(prepared, assignedFreeValues, assignedFreeMask, 0, allSolutions);

        std::ranges::sort(allSolutions, [](solution const &lhs, solution const &rhs) {
            if (lhs.sum != rhs.sum) { return lhs.sum < rhs.sum; }
            return lhs.values < rhs.values;
        });

        return allSolutions;
    }

private:
    std::vector<equation>                    m_equations;
    std::vector<std::string>                m_variableNames;
    std::unordered_map<std::string, size_t> m_variableIdByName;

    // Interns a variable name into a compact numeric id used by the search code.
    [[nodiscard]] size_t
    get_orCreateVariableId(std::string_view variableName) {
        auto variableNameStr = std::string(variableName);
        if (auto found = m_variableIdByName.find(variableNameStr); found != m_variableIdByName.end()) {
            return found->second;
        }

        auto const newId = m_variableNames.size();
        m_variableNames.push_back(variableNameStr);
        m_variableIdByName.emplace(std::move(variableNameStr), newId);
        return newId;
    }

    // Converts the equation system into exact row-reduced echelon form using rational arithmetic.
    void
    reduce_to_rref(std::vector<std::vector<rational>> &matrix, std::vector<int> &pivotRowByVariable) const {
        size_t pivotRow = 0;
        for (size_t columnId = 0; columnId < pivotRowByVariable.size() && pivotRow < matrix.size(); ++columnId) {
            auto candidateRow = pivotRow;
            while (candidateRow < matrix.size() && matrix.at(candidateRow).at(columnId).is_zero()) { candidateRow++; }
            if (candidateRow == matrix.size()) { continue; }

            std::swap(matrix.at(pivotRow), matrix.at(candidateRow));

            auto const pivotValue = matrix.at(pivotRow).at(columnId);
            for (auto &cell : matrix.at(pivotRow)) { cell = cell / pivotValue; }

            for (size_t rowId = 0; rowId < matrix.size(); ++rowId) {
                if (rowId == pivotRow || matrix.at(rowId).at(columnId).is_zero()) { continue; }

                auto const factor = matrix.at(rowId).at(columnId);
                for (size_t innerColumnId = columnId; innerColumnId < matrix.at(rowId).size(); ++innerColumnId) {
                    matrix.at(rowId).at(innerColumnId) =
                        matrix.at(rowId).at(innerColumnId) - factor * matrix.at(pivotRow).at(innerColumnId);
                }
            }

            pivotRowByVariable.at(columnId) = static_cast<int>(pivotRow);
            pivotRow++;
        }
    }

    // Reduces the original equations to row-reduced echelon form and extracts pivot-variable expressions
    // in terms of the free variables. Search then only needs to enumerate the free variables.
    [[nodiscard]] prepared_problem
    prepare_problem() const {
        prepared_problem result;
        if (m_equations.empty() || m_variableNames.empty()) {
            result.isValid = false;
            return result;
        }

        std::vector<int> occurrences(m_variableNames.size(), 0);
        int              globalUpperBound = 0;
        std::vector<std::vector<rational>> reducedMatrix(
            m_equations.size(), std::vector<rational>(m_variableNames.size() + 1, rational{}));

        for (size_t rowId = 0; rowId < m_equations.size(); ++rowId) {
            auto const &equation = m_equations.at(rowId);
            if (not equation.isClosed || equation.variableIds.empty() || equation.rhs < 0) {
                result.isValid = false;
                return result;
            }

            globalUpperBound = std::max(globalUpperBound, equation.rhs);
        }

        result.staticUpperBounds = std::vector<int>(m_variableNames.size(), globalUpperBound);

        for (size_t rowId = 0; rowId < m_equations.size(); ++rowId) {
            auto const &equation = m_equations.at(rowId);

            std::vector<int> localCoefficients(m_variableNames.size(), 0);
            for (auto variableId : equation.variableIds) {
                localCoefficients.at(variableId)++;
                occurrences.at(variableId)++;
            }

            for (size_t variableId = 0; variableId < localCoefficients.size(); ++variableId) {
                auto const coefficient = localCoefficients.at(variableId);
                if (coefficient == 0) { continue; }

                auto const maxValue = equation.rhs / coefficient;
                result.staticUpperBounds.at(variableId) = std::min(result.staticUpperBounds.at(variableId), maxValue);
                reducedMatrix.at(rowId).at(variableId) = rational(coefficient);
            }

            reducedMatrix.at(rowId).back() = rational(equation.rhs);
        }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            if (result.staticUpperBounds.at(variableId) < 0 || occurrences.at(variableId) == 0) {
                result.isValid = false;
                return result;
            }
        }

        auto pivotRowByVariable = std::vector<int>(m_variableNames.size(), -1);
        reduce_to_rref(reducedMatrix, pivotRowByVariable);

        for (auto const &row : reducedMatrix) {
            auto allCoefficientsZero = true;
            for (size_t columnId = 0; columnId < m_variableNames.size(); ++columnId) {
                if (not row.at(columnId).is_zero()) {
                    allCoefficientsZero = false;
                    break;
                }
            }

            if (allCoefficientsZero && not row.back().is_zero()) {
                result.isValid = false;
                return result;
            }
        }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            if (pivotRowByVariable.at(variableId) == -1) { result.variableOrder.push_back(variableId); }
            else { result.pivotVariables.push_back(variableId); }
        }

        std::ranges::sort(result.variableOrder, [&](size_t lhs, size_t rhs) {
            if (result.staticUpperBounds.at(lhs) != result.staticUpperBounds.at(rhs)) {
                return result.staticUpperBounds.at(lhs) < result.staticUpperBounds.at(rhs);
            }
            if (occurrences.at(lhs) != occurrences.at(rhs)) { return occurrences.at(lhs) > occurrences.at(rhs); }
            return m_variableNames.at(lhs) < m_variableNames.at(rhs);
        });

        result.freeIndexByVariable = std::vector<int>(m_variableNames.size(), -1);
        for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
            result.freeIndexByVariable.at(result.variableOrder.at(freeIndex)) = static_cast<int>(freeIndex);
        }

        result.variableExpressions = std::vector<affine_expression>(m_variableNames.size());
        result.objectiveWeights    = std::vector<rational>(result.variableOrder.size(), rational{});
        for (auto &expression : result.variableExpressions) {
            expression.freeCoefficients = std::vector<rational>(result.variableOrder.size(), rational{});
        }

        for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
            auto const variableId = result.variableOrder.at(freeIndex);
            result.variableExpressions.at(variableId).freeCoefficients.at(freeIndex) = rational(1);
        }

        for (auto pivotVariableId : result.pivotVariables) {
            auto const pivotRow   = static_cast<size_t>(pivotRowByVariable.at(pivotVariableId));
            auto      &expression = result.variableExpressions.at(pivotVariableId);

            expression.constant = reducedMatrix.at(pivotRow).back();
            for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
                auto const freeVariableId = result.variableOrder.at(freeIndex);
                expression.freeCoefficients.at(freeIndex) = -reducedMatrix.at(pivotRow).at(freeVariableId);
            }
        }

        for (auto const &expression : result.variableExpressions) {
            result.objectiveConstant = result.objectiveConstant + expression.constant;
            for (size_t freeIndex = 0; freeIndex < result.variableOrder.size(); ++freeIndex) {
                result.objectiveWeights.at(freeIndex) =
                    result.objectiveWeights.at(freeIndex) + expression.freeCoefficients.at(freeIndex);
            }
        }

        return result;
    }

    // Computes the best possible objective value reachable from the current partial assignment.
    // Unassigned free variables are given whichever endpoint would help the objective most.
    [[nodiscard]] bool
    is_feasible(prepared_problem const &prepared, std::vector<int> const &assignedFreeValues,
                std::vector<bool> const &assignedFreeMask, std::optional<int> bestSum) const {
        auto optimisticObjective = prepared.objectiveConstant;
        for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
            if (assignedFreeMask.at(freeIndex)) {
                optimisticObjective = optimisticObjective +
                                      prepared.objectiveWeights.at(freeIndex) * assignedFreeValues.at(freeIndex);
                continue;
            }

            if (prepared.objectiveWeights.at(freeIndex) < rational{}) {
                optimisticObjective = optimisticObjective +
                                      prepared.objectiveWeights.at(freeIndex) *
                                          prepared.staticUpperBounds.at(prepared.variableOrder.at(freeIndex));
            }
        }

        if (bestSum.has_value() && optimisticObjective >= rational(bestSum.value())) { return false; }

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            auto valueMin = prepared.variableExpressions.at(variableId).constant;
            auto valueMax = prepared.variableExpressions.at(variableId).constant;

            for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
                auto const coefficient = prepared.variableExpressions.at(variableId).freeCoefficients.at(freeIndex);
                if (coefficient.is_zero()) { continue; }

                if (assignedFreeMask.at(freeIndex)) {
                    auto const contribution = coefficient * assignedFreeValues.at(freeIndex);
                    valueMin = valueMin + contribution;
                    valueMax = valueMax + contribution;
                    continue;
                }

                auto const freeUpperBound = prepared.staticUpperBounds.at(prepared.variableOrder.at(freeIndex));
                if (coefficient > rational{}) { valueMax = valueMax + coefficient * freeUpperBound; }
                else { valueMin = valueMin + coefficient * freeUpperBound; }
            }

            if (valueMax < rational{}) { return false; }
            if (valueMin > rational(prepared.staticUpperBounds.at(variableId))) { return false; }
        }

        return true;
    }

    // Evaluates the reduced solution representation for a complete free-variable assignment.
    [[nodiscard]] std::optional<solution>
    materialize_solution(prepared_problem const &prepared, std::vector<int> const &assignedFreeValues) const {
        solution result;
        int      sum = 0;

        for (size_t variableId = 0; variableId < m_variableNames.size(); ++variableId) {
            auto value = prepared.variableExpressions.at(variableId).constant;
            for (size_t freeIndex = 0; freeIndex < prepared.variableOrder.size(); ++freeIndex) {
                value = value + prepared.variableExpressions.at(variableId).freeCoefficients.at(freeIndex) *
                                    assignedFreeValues.at(freeIndex);
            }

            if (not value.is_integer()) { return std::nullopt; }

            auto const integerValue = value.as_integer();
            if (integerValue < 0 || integerValue > prepared.staticUpperBounds.at(variableId)) { return std::nullopt; }

            result.values.emplace(m_variableNames.at(variableId), static_cast<int>(integerValue));
            sum += static_cast<int>(integerValue);
        }

        result.sum = sum;
        return result;
    }

    [[nodiscard]] bool
    is_better_solution(solution const &candidate, std::optional<solution> const &bestSolution) const {
        if (not bestSolution.has_value()) { return true; }
        if (candidate.sum != bestSolution->sum) { return candidate.sum < bestSolution->sum; }
        return candidate.values < bestSolution->values;
    }

    // Depth-first search over free variables only. Pivot variables are reconstructed afterwards.
    void
    search_minSum(prepared_problem const &prepared, std::vector<int> &assignedFreeValues,
                  std::vector<bool> &assignedFreeMask, size_t depth, std::optional<solution> &bestSolution,
                  std::optional<int> &bestSum) const {
        if (not is_feasible(prepared, assignedFreeValues, assignedFreeMask, bestSum)) { return; }

        if (depth == prepared.variableOrder.size()) {
            auto candidate = materialize_solution(prepared, assignedFreeValues);
            if (candidate.has_value() && is_better_solution(candidate.value(), bestSolution)) {
                bestSolution = candidate;
                bestSum      = bestSolution->sum;
            }
            return;
        }

        auto const variableId = prepared.variableOrder.at(depth);
        auto const freeIndex  = static_cast<size_t>(prepared.freeIndexByVariable.at(variableId));
        auto const upperBound = prepared.staticUpperBounds.at(variableId);

        if (prepared.objectiveWeights.at(freeIndex) < rational{}) {
            for (int candidateValue = upperBound; candidateValue >= 0; --candidateValue) {
                assignedFreeValues.at(freeIndex) = candidateValue;
                assignedFreeMask.at(freeIndex)   = true;
                search_minSum(prepared, assignedFreeValues, assignedFreeMask, depth + 1, bestSolution, bestSum);
                assignedFreeMask.at(freeIndex) = false;
            }
        }
        else {
            for (int candidateValue = 0; candidateValue <= upperBound; ++candidateValue) {
                assignedFreeValues.at(freeIndex) = candidateValue;
                assignedFreeMask.at(freeIndex)   = true;
                search_minSum(prepared, assignedFreeValues, assignedFreeMask, depth + 1, bestSolution, bestSum);
                assignedFreeMask.at(freeIndex) = false;
            }
        }

        assignedFreeValues.at(freeIndex) = 0;
    }

    // Enumerates all feasible assignments for the free variables and reconstructs the pivot variables.
    void
    search_all(prepared_problem const &prepared, std::vector<int> &assignedFreeValues,
               std::vector<bool> &assignedFreeMask, size_t depth, std::vector<solution> &allSolutions) const {
        if (not is_feasible(prepared, assignedFreeValues, assignedFreeMask, std::nullopt)) { return; }

        if (depth == prepared.variableOrder.size()) {
            auto candidate = materialize_solution(prepared, assignedFreeValues);
            if (candidate.has_value()) { allSolutions.push_back(candidate.value()); }
            return;
        }

        auto const variableId = prepared.variableOrder.at(depth);
        auto const freeIndex  = static_cast<size_t>(prepared.freeIndexByVariable.at(variableId));
        auto const upperBound = prepared.staticUpperBounds.at(variableId);

        for (int candidateValue = 0; candidateValue <= upperBound; ++candidateValue) {
            assignedFreeValues.at(freeIndex) = candidateValue;
            assignedFreeMask.at(freeIndex)   = true;
            search_all(prepared, assignedFreeValues, assignedFreeMask, depth + 1, allSolutions);
            assignedFreeMask.at(freeIndex) = false;
        }

        assignedFreeValues.at(freeIndex) = 0;
    }
};

}
}
