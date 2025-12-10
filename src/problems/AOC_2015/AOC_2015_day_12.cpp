#include <fstream>
#include <glaze/json/generic.hpp>
#include <queue>

#include <incom_commons.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <glaze/glaze.hpp>

namespace AOC2015 {
int day12_1(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string oneStr;
    glz::generic json{};

    while (std::getline(iStream, oneStr)) { auto ec = glz::read_json(json, oneStr); }

    std::queue<glz::generic *> q;
    q.push(&json);

    int ans = 0;
    while (not q.empty()) {
        std::visit(overloaded{[&](auto &a) {}, [&](double &a) { ans += a; },
                              [&](std::vector<glz::generic> &a) {
                                  for (auto &jsonItem : a) { q.push(&jsonItem); }
                              },
                              [&](std::map<std::string, glz::generic, std::less<>> &a) {
                                  for (auto &[__, jsonItem] : a) { q.push(&jsonItem); }
                              }},
                   q.front()->data);
        q.pop();
    }
    return ans;
}

int day12_2(std::string dataFile) {
    std::ifstream iStream;
    iStream.clear();
    iStream.open(dataFile);
    if (not iStream.is_open()) { return -1; }

    std::string oneStr;

    glz::generic json{};

    while (std::getline(iStream, oneStr)) { auto ec = glz::read_json(json, oneStr); }

    std::queue<glz::generic *> q;
    q.push(&json);

    int ans = 0;

    while (not q.empty()) {
        std::visit(overloaded{[&](auto &a) {}, [&](double &a) { ans += a; },
                              [&](std::vector<glz::generic> &a) {
                                  for (auto &jsonItem : a) { q.push(&jsonItem); }
                              },
                              [&](std::map<std::string, glz::generic, std::less<>> &a) {
                                  bool enable = true;
                                  for (auto &[__, jsonItem] : a) {
                                      std::visit(overloaded{[&](auto &it) {},
                                                            [&](std::string &it) {
                                                                if (it == "red") { enable = false; }
                                                            }},
                                                 jsonItem.data);
                                  }
                                  if (enable) {
                                      for (auto &[__, jsonItem] : a) { q.push(&jsonItem); }
                                  }
                              }},
                   q.front()->data);

        q.pop();
    }
    return ans;
}
} // namespace AOC2015