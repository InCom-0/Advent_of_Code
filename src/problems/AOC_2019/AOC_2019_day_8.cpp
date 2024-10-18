#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <iostream>

namespace AOC2019 {
long long day8_1(std::string dataFile, std::pair<size_t, size_t> imgSizeWH) {

    struct ImgLayer {
        std::vector<std::vector<int>> img;
    };
    auto d_ctre = ctre::search<R"(\d+)">;

    auto             inputStr = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front();
    std::vector<int> intInput;
    for (auto &chr : inputStr) { intInput.push_back(chr - 48); }

    size_t                curLayer = 0;
    std::vector<ImgLayer> layers;


    while (curLayer * imgSizeWH.first * imgSizeWH.second < inputStr.size()) {
        layers.push_back(ImgLayer{});
        for (int i = 0; i < imgSizeWH.second; ++i) {
            layers.back().img.push_back(std::vector<int>(
                intInput.begin() + (i * imgSizeWH.first) + (curLayer * imgSizeWH.first * imgSizeWH.second),
                intInput.begin() + ((i + 1) * imgSizeWH.first) + (curLayer * imgSizeWH.first * imgSizeWH.second)));
        }
        curLayer++;
    }

    auto minCursor = flux::ref(layers).find_min([](auto &&a, auto &&b) {
        auto y =
            flux::ref(a.img).fold([](auto &&init, auto &&a_line) { return flux::ref(a_line).count_eq(0) + init; }, 0);
        auto z =
            flux::ref(b.img).fold([](auto &&init, auto &&a_line) { return flux::ref(a_line).count_eq(0) + init; }, 0);
        return y < z;
    });

    auto &minItem = flux::read_at(layers, minCursor);

    auto oneDigi =
        flux::ref(minItem.img).fold([](auto &&init, auto &&a) { return flux::ref(a).count_eq(1) + init; }, 0);

    auto oneDig2 =
        flux::ref(minItem.img).fold([](auto &&init, auto &&a) { return flux::ref(a).count_eq(2) + init; }, 0);

    return oneDigi * oneDig2;
}

std::string day8_2(std::string dataFile, std::pair<size_t, size_t> imgSizeWH) {
    struct ImgLayer {
        std::vector<std::vector<int>> img;
    };
    auto d_ctre = ctre::search<R"(\d+)">;

    auto             inputStr = incom::commons::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre).front().front();
    std::vector<int> intInput;
    for (auto &chr : inputStr) { intInput.push_back(chr - 48); }

    size_t                curLayer = 0;
    std::vector<ImgLayer> layers;


    while (curLayer * imgSizeWH.first * imgSizeWH.second < inputStr.size()) {
        layers.push_back(ImgLayer{});
        for (int i = 0; i < imgSizeWH.second; ++i) {
            layers.back().img.push_back(std::vector<int>(
                intInput.begin() + (i * imgSizeWH.first) + (curLayer * imgSizeWH.first * imgSizeWH.second),
                intInput.begin() + ((i + 1) * imgSizeWH.first) + (curLayer * imgSizeWH.first * imgSizeWH.second)));
        }
        curLayer++;
    }

    ImgLayer ans{std::vector<std::vector<int>>(imgSizeWH.second, std::vector<int>(imgSizeWH.first, 2))};

    for (auto r_iter = layers.rbegin(); r_iter != layers.rend(); r_iter++) {
        auto &selected = *r_iter;
        for (int h = 0; h < imgSizeWH.second; ++h) {
            for (int w = 0; w < imgSizeWH.first; ++w) {
                if (selected.img[h][w] < 2) { ans.img[h][w] = selected.img[h][w]; }
            }
        }
    }

    for (auto &line : ans.img) {
        for (auto &num : line) { std::cout << (num == 0 ? ' ' : '#'); }
        std::cout << '\n';
    }
    return "HCGFE";
}
} // namespace AOC2019