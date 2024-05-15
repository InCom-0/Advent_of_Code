#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day3_1(std::string dataFile) {
        using namespace std;

        std::ifstream iStream;
        iStream.clear();
        iStream.open(dataFile);

        char oneChar;

        if (not iStream.is_open()) return -1;

        unsigned int posX = USHRT_MAX / 2;
        unsigned int posY = posX;
        unsigned int posX2 = posX;
        unsigned int posY2 = posY;

        int accu = 1;

        vector<vector<bool> > dp(USHRT_MAX, vector<bool>(USHRT_MAX, false));
        dp[posX][posY] = true;

        std::vector<int> lwh;

        while (iStream >> oneChar) {
            switch (oneChar) {
                case '>':
                    posX++;
                break;

                case 'v':
                    posY--;
                break;

                case '<':
                    posX--;
                break;

                case '^':
                    posY++;
                break;
            }

            if (dp[posX][posY] == false) accu++;
            dp[posX][posY] = true;

            std::swap(posX, posX2);
            std::swap(posY, posY2);
        }

        iStream.clear();
        iStream.close();

        return accu;
    }
}