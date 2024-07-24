#include <AOC_commons.h>
#include <bitlib/bitlib.hpp>
#include <ctre.hpp>
#include <flux.hpp>


namespace AOC2018 {
long long day14_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input = std::stoi(AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre).front().front());

    std::vector<unsigned int> line{3, 7};
    line.reserve(input + 11);

    int posA = 0;
    int posB = 1;
    while (line.size() < (input + 10)) {
        if (posA == posB) { return INT_MIN; }

        int sumNum = line[posA] + line[posB];
        if (sumNum / 10 != 0) { line.push_back(sumNum / 10); }
        line.push_back(sumNum % 10);

        posA = (posA + line[posA] + 1) % line.size();
        posB = (posB + line[posB] + 1) % line.size();
    }

    long long ans        = 0;
    long long multiplier = 1;
    for (int i = input + 9; i >= input; --i) {
        ans        += line[i] * multiplier;
        multiplier *= 10;
    }
    return ans;
}

long long day14_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre).front().front();

    std::vector<unsigned int> line{3, 7};
    line.reserve(33'554'432);

    int         posA = 0;
    int         posB = 1;
    std::string curEva(input.size(), 'X');
    while (true) {
        if (posA == posB) { return INT_MIN; }

        int sumNum = line[posA] + line[posB];
        if (sumNum / 10 != 0) {
            line.push_back(sumNum / 10);
            if (line.back() + 48 == input[curEva.size()]) { curEva.push_back(input[curEva.size()]); }
            else { curEva.clear(); }

            if (curEva.size() == input.size()) { return line.size() - curEva.size(); }
        }

        line.push_back(sumNum % 10);
        if (line.back() + 48 == input[curEva.size()]) { curEva.push_back(input[curEva.size()]); }
        else { curEva.clear(); }

        if (curEva.size() == input.size()) { return line.size() - curEva.size(); }

        posA = (posA + line[posA] + 1) % line.size();
        posB = (posB + line[posB] + 1) % line.size();
    }
    std::unreachable();
}


long long day14_3(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre).front().front();

    std::vector<unsigned int> line{3, 7};
    line.reserve(33'554'432);

    std::string curEva(input.size(), 'X');
    int         posA = 0;
    int         posB = 1;

    while (true) {
        if (posA == posB) { return INT_MIN; }

        int sumNum = line[posA] + line[posB];
        if (sumNum / 10 != 0) {
            line.push_back(sumNum / 10);
            std::rotate(curEva.begin(), curEva.begin() + 1, curEva.end());
            curEva.pop_back();
            curEva.push_back(line.back() + 48);

            if (curEva == input) { return line.size() - curEva.size(); }
        }

        line.push_back(sumNum % 10);
        std::rotate(curEva.begin(), curEva.begin() + 1, curEva.end());
        curEva.pop_back();
        curEva.push_back(line.back() + 48);

        if (curEva == input) { return line.size() - curEva.size(); }

        posA = (posA + line[posA] + 1) % line.size();
        posB = (posB + line[posB] + 1) % line.size();
    }
    std::unreachable();
}
long long day14_4(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre).front().front();

    std::vector<std::string> strRange(input.size(), std::string(input.size(), 'X'));

    for (int i = 1; i < input.size(); i++) {
        std::rotate_copy(input.rbegin(), input.rbegin() + i, input.rend(), strRange[i - 1].rbegin());
    }
    strRange.back() = input;

    std::vector<unsigned int> line{3, 7};
    line.reserve(33'554'432);

    std::string curEva(input.size(), 'X');
    int         posA         = 0;
    int         posB         = 1;
    int         posWithinCur = 0;

    while (true) {
        // if (posA == posB) { return INT_MIN; }

        int sumNum = line[posA] + line[posB];
        if (sumNum / 10 != 0) {
            line.push_back(sumNum / 10);
            curEva[posWithinCur] = line.back() + 48;

            if (curEva == strRange[posWithinCur]) { return line.size() - curEva.size(); }
            posWithinCur++;
            if (posWithinCur == curEva.size()) { posWithinCur = 0; }
        }

        line.push_back(sumNum % 10);
        curEva[posWithinCur] = line.back() + 48;

        if (curEva == strRange[posWithinCur]) { return line.size() - curEva.size(); }
        posWithinCur++;
        if (posWithinCur == curEva.size()) { posWithinCur = 0; }

        posA = (posA + line[posA] + 1) % line.size();
        posB = (posB + line[posB] + 1) % line.size();

        // if (line.size() > 20333868) { std::cout << line.size() << '\n'; }
    }
    std::unreachable();
}
long long day14_5(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = AOC_commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre).front().front();

    std::vector<std::string> strRange(input.size(), std::string(input.size(), 'X'));

    for (int i = 1; i < input.size(); i++) {
        std::rotate_copy(input.rbegin(), input.rbegin() + i, input.rend(), strRange[i - 1].rbegin());
    }
    strRange.back() = input;

    std::vector<unsigned int> line{3, 7};
    line.reserve(33'554'432);

    std::string curEva(input.size(), 'X');
    int         posA         = 0;
    int         posB         = 1;
    int         posWithinCur = 0;

    while (true) {
        int sumNum = line[posA] + line[posB];
        if (sumNum / 10 != 0) {
            line.push_back(sumNum / 10);
            curEva[posWithinCur] = line.back() + 48;

            if (curEva == strRange[posWithinCur]) { return line.size() - curEva.size(); }
            posWithinCur++;
            if (posWithinCur == curEva.size()) { posWithinCur = 0; }
        }

        line.push_back(sumNum % 10);
        curEva[posWithinCur] = line.back() + 48;

        if (curEva == strRange[posWithinCur]) { return line.size() - curEva.size(); }
        posWithinCur++;
        if (posWithinCur == curEva.size()) { posWithinCur = 0; }

        posA = posA + line[posA] + 1;
        posB = posB + line[posB] + 1;
        if (posA >= line.size()) { posA = posA % line.size(); }
        if (posB >= line.size()) { posB = posB % line.size(); }
    }
    std::unreachable();
}
} // namespace AOC2018