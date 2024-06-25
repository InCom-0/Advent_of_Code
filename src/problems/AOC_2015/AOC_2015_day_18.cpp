#include <iostream>
#include <fstream>
#include <queue>

#include <AOC_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day18_1(std::string dataFile, int dimension, int repeatCount) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        std::vector<int> oneLine;
        std::string oneStr;

        std::string lastRe = "";
        std::vector<std::vector<int> > ___1;
        std::vector<std::vector<int> > ___2(dimension + 2, std::vector<int>(dimension + 3, 0));

        std::vector<std::vector<int> > *matrix_A = &___1;
        std::vector<std::vector<int> > *matrix_B = &___2;

        matrix_A->push_back(std::vector<int>(dimension + 3, 0));

        while (std::getline(iStream, oneStr)) {
            oneLine.push_back(0);
            oneLine.push_back(0);
            for (const auto &charItem: oneStr) {
                if (charItem == '#') oneLine.push_back(1);
                else oneLine.push_back(0);
            }
            oneLine.push_back(0);
            matrix_A->push_back(oneLine);
            oneLine.clear();
        }
        matrix_A->push_back(std::vector<int>(dimension + 3, 0));
        std::vector<int> zeros(dimension + 3, 0);

        int threeItems;

        for (int r = 0; r < repeatCount; ++r) {
            for (int i = 1; i < dimension + 1; ++i) {
                threeItems = matrix_A->at(i)[1] + matrix_A->at(i)[2];
                for (int j = 2; j < dimension + 2; ++j) {
                    threeItems += (matrix_A->at(i)[j + 1] - matrix_A->at(i)[j - 2]);
                    matrix_B->at(i - 1)[j] += threeItems;
                    matrix_B->at(i + 1)[j] += threeItems;
                    matrix_B->at(i)[j - 1] += matrix_A->at(i)[j];
                    matrix_B->at(i)[j + 1] += matrix_A->at(i)[j];
                }
            }

            for (int i = 1; i < dimension + 1; ++i) {
                for (int j = 2; j < dimension + 2; ++j) {
                    if (matrix_A->at(i)[j] == 1) {
                        if (matrix_B->at(i)[j] < 2 || matrix_B->at(i)[j] > 3) matrix_A->at(i)[j] = 0;
                    } else {
                        if (matrix_B->at(i)[j] == 3) matrix_A->at(i)[j] = 1;
                    }
                }
                matrix_B->at(i) = zeros;
            }
        }
        auto ans = flux::ref(*matrix_A)
                .fold([](int &&ini, auto &&a) {
                    return ini + flux::ref(a).count_if([](auto &&b) { return b == 1; });
                }, 0);


        return ans;
    }
    int day18_2(std::string dataFile, int dimension, int repeatCount) {
        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);
        if (not iStream.is_open()) return -1;

        std::vector<int> oneLine;
        std::string oneStr;

        std::string lastRe = "";
        std::vector<std::vector<int> > ___1;
        std::vector<std::vector<int> > ___2(dimension + 2, std::vector<int>(dimension + 3, 0));

        std::vector<std::vector<int> > *matrix_A = &___1;
        std::vector<std::vector<int> > *matrix_B = &___2;

        matrix_A->push_back(std::vector<int>(dimension + 3, 0));

        while (std::getline(iStream, oneStr)) {
            oneLine.push_back(0);
            oneLine.push_back(0);
            for (const auto &charItem: oneStr) {
                if (charItem == '#') oneLine.push_back(1);
                else oneLine.push_back(0);
            }
            oneLine.push_back(0);
            matrix_A->push_back(oneLine);
            oneLine.clear();
        }
        matrix_A->push_back(std::vector<int>(dimension + 3, 0));
        std::vector<int> zeros(dimension + 3, 0);

        int threeItems;

        for (int r = 0; r < repeatCount; ++r) {
            matrix_A->at(1)[2] = 1;
            matrix_A->at(1)[dimension + 1] = 1;
            matrix_A->at(dimension)[2] = 1;
            matrix_A->at(dimension)[dimension + 1] = 1;

            for (int i = 1; i < dimension + 1; ++i) {
                threeItems = matrix_A->at(i)[1] + matrix_A->at(i)[2];
                for (int j = 2; j < dimension + 2; ++j) {
                    threeItems += (matrix_A->at(i)[j + 1] - matrix_A->at(i)[j - 2]);
                    matrix_B->at(i - 1)[j] += threeItems;
                    matrix_B->at(i + 1)[j] += threeItems;
                    matrix_B->at(i)[j - 1] += matrix_A->at(i)[j];
                    matrix_B->at(i)[j + 1] += matrix_A->at(i)[j];
                }
            }

            for (int i = 1; i < dimension + 1; ++i) {
                for (int j = 2; j < dimension + 2; ++j) {
                    if (matrix_A->at(i)[j] == 1) {
                        if (matrix_B->at(i)[j] < 2 || matrix_B->at(i)[j] > 3) matrix_A->at(i)[j] = 0;
                    } else {
                        if (matrix_B->at(i)[j] == 3) matrix_A->at(i)[j] = 1;
                    }
                }
                matrix_B->at(i) = zeros;
            }
        }

        matrix_A->at(1)[2] = 1;
        matrix_A->at(1)[dimension + 1] = 1;
        matrix_A->at(dimension)[2] = 1;
        matrix_A->at(dimension)[dimension + 1] = 1;

        auto ans = flux::ref(*matrix_A)
                .fold([](int &&ini, auto &&a) {
                    return ini + flux::ref(a).count_if([](auto &&b) { return b == 1; });
                }, 0);


        return ans;
    }
}