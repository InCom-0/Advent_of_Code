#include <ankerl/unordered_dense.h>
#include <ctre.hpp>
#include <flux.hpp>
#include <incom_commons.h>
#include <ranges>
#include <string>


namespace AOC2025 {

namespace {

size_t
pow10(size_t exp) {
    size_t value = 1;
    while (exp-- > 0) { value *= 10; }
    return value;
}

size_t
sumInclusive(size_t from, size_t upto) {
    size_t const count = upto - from + 1;
    size_t const ends  = from + upto;
    if ((count % 2) == 0) { return (count / 2) * ends; }
    return count * (ends / 2);
}

} // namespace

size_t
day2_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre, d_ctre);

    struct Cur {
        size_t num;
        size_t l;
        size_t r;
        size_t len;

        Cur(std::string const &str)
            : num(std::stoull(str)), len(str.size()),
              l(std::stoull(std::string(str.begin(), str.begin() + str.size() / 2))),
              r(std::stoull(std::string(str.begin() + str.size() / 2, str.end()))) {}
        void
        next() {
            len++;
            if ((len % 2) == 1) { len++; }
            l   = std::pow(10uz, (len / 2uz) - 1);
            r   = l;
            num = (l * std::pow(10uz, (len / 2uz))) + r;
        }
    };

    auto calc2 = [&](std::pair<std::string &, std::string &> const &rf) {
        auto      start   = Cur(rf.first);
        auto      end     = Cur(rf.second);
        long long resAccu = 0ll;

        if ((start.len % 2) == 1) { start.next(); }
        else if (start.l > start.r) {
            start.r   = start.l;
            start.num = start.l * std::pow(10uz, (start.len / 2uz)) + start.r;
        }
        else if (start.l < start.r) {
            start.l++;
            start.r   = start.l;
            start.num = start.l * std::pow(10uz, (start.len / 2uz)) + start.r;
        }

        while (start.num <= end.num) {
            size_t ceil = 0uz;
            if (start.len == end.len) { ceil = end.num + 1; }
            else { ceil = std::pow(10uz, start.len); }

            size_t const adder = std::pow(10uz, (start.len / 2uz)) + 1;
            size_t const mult  = (ceil - start.num) / adder;

            resAccu += (start.num * (mult + 1));
            resAccu += ((mult * adder) * ((mult + 1) / 2.0));
            if (start.len == end.len) { break; }
            start.next();
        }
        return resAccu;
    };

    size_t res = 0uz;
    for (std::pair<std::string &, std::string &> rf : std::views::zip(input.at(0), input.at(1))) { res += calc2(rf); }
    return res;
}

size_t
day2_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\d+)">;
    auto input  = incom::commons::parseInputUsingCTRE::processOneLineRPTinFile(dataFile, d_ctre, d_ctre);

    auto parseNum = [](std::string const &str) { return std::stoull(str); };

    auto repeatValue = [](size_t block, size_t blockLen, size_t repeatCount) {
        size_t const mult = pow10(blockLen);
        size_t       out  = 0;
        for (size_t i = 0; i < repeatCount; ++i) { out = (out * mult) + block; }
        return out;
    };

    auto ceilDiv = [](size_t a, size_t b) { return (a + b - 1) / b; };

    size_t res = 0uz;
    for (std::pair<std::string &, std::string &> rf : std::views::zip(input.at(0), input.at(1))) {
        size_t const rangeStart = parseNum(rf.first);
        size_t const rangeEnd   = parseNum(rf.second);
        size_t const lenStart   = rf.first.size();
        size_t const lenEnd     = rf.second.size();

        ankerl::unordered_dense::set<size_t> uniqueInvalid{};

        for (size_t len = lenStart; len <= lenEnd; ++len) {
            size_t const lo = len == lenStart ? rangeStart : pow10(len - 1);
            size_t const hi = len == lenEnd ? rangeEnd : pow10(len) - 1;
            if (lo > hi || len < 2) { continue; }

            for (size_t repeatCount = 2; repeatCount <= len; ++repeatCount) {
                if ((len % repeatCount) != 0) { continue; }

                size_t const blockLen = len / repeatCount;
                size_t const blockMin = pow10(blockLen - 1);
                size_t const blockMax = pow10(blockLen) - 1;
                size_t const shift    = pow10(blockLen);
                size_t       coeff    = 0;
                for (size_t i = 0; i < repeatCount; ++i) { coeff = (coeff * shift) + 1; }

                size_t const from = std::max(blockMin, ceilDiv(lo, coeff));
                size_t const upto = std::min(blockMax, hi / coeff);
                if (from > upto) { continue; }

                for (size_t block = from; block <= upto; ++block) {
                    uniqueInvalid.insert(repeatValue(block, blockLen, repeatCount));
                }
            }
        }

        for (size_t const id : uniqueInvalid) { res += id; }
    }
    return res;


    return 0uz;
}

} // namespace AOC2025