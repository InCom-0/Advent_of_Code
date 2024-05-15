#include <iostream>
#include <fstream>
#include <queue>

#include <LC_commons.h>
#include <ctre.hpp>
#include "flux.hpp"
#include <glaze/glaze.hpp>

namespace AOC2015 {
    int day12_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;

    std::string oneStr;
    glz::json_t json{};

    while (std::getline(iStream, oneStr)) {
        auto ec = glz::read_json(json, oneStr);
    }

    std::queue<glz::json_t *> q;
    q.push(&json);

    int ans = 0;
    while (not q.empty()) {
        std::visit(overloaded{
                       [&](auto &a) {
                       },
                       [&](double &a) { ans += a; },
                       [&](std::vector<glz::json_t> &a) { for (auto &jsonItem: a) q.push(&jsonItem); },
                       [&](std::map<std::string, glz::json_t, std::less<> > &a) {
                           for (auto &[__,jsonItem]: a) q.push(&jsonItem);
                       }
                   }, q.front()->data);
        q.pop();
    }
    return ans;
}

int day12_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) return -1;

    std::string oneStr;

    glz::json_t json{};

    while (std::getline(iStream, oneStr)) {
        auto ec = glz::read_json(json, oneStr);
    }

    std::queue<glz::json_t *> q;
    q.push(&json);

    int ans = 0;

    while (not q.empty()) {
        std::visit(overloaded{
                       [&](auto &a) {
                       },
                       [&](double &a) { ans += a; },
                       [&](std::vector<glz::json_t> &a) { for (auto &jsonItem: a) q.push(&jsonItem); },
                       [&](std::map<std::string, glz::json_t, std::less<> > &a) {
                           bool enable = true;
                           for (auto &[__,jsonItem]: a) {
                               std::visit(overloaded{
                                              [&](auto &it) {
                                              },
                                              [&](std::string &it) { if (it == "red") enable = false; }
                                          }, jsonItem.data);
                           }
                           if (enable) for (auto &[__,jsonItem]: a) q.push(&jsonItem);
                       }
                   }, q.front()->data);

        q.pop();
    }
    return ans;
}
}