#pragma once

#include <vector>
#include <fstream>
#include <ctre.hpp>
#include "xxhash.h"


template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

namespace AOC_commons {

    // This isn't great as it doesn't take into account types with dynamically allocated content (typically containers).
    struct XXH3Hasher {
        size_t operator() (auto &&input) const {
            return XXH3_64bits(&input, sizeof(input));
        }
    };

    class parseInputUsingCTRE {
    private:
        struct oneLineProcessedHolder {
            std::vector<std::string> insideVofS;
            size_t cursor = 0;
            int somethingNotFoundAt = -1;

            oneLineProcessedHolder &operator<<(auto &&toInsert) {
                if (toInsert) insideVofS.push_back(toInsert.to_string());
                else somethingNotFoundAt = cursor;
                cursor++;
                return *this;
            }
        };

        /*
        Questionable necessity to use a template for fileProcessedHolder.
        Only did it to learn how to deal with parameter pack 'sizeof...' and constexpr context.

        Thought came to mind ... would it be a good idea to implement this using some sort of flux sequence?
        That might be nicer && might avoid using that crude 'cursor' and actually learn to use a well made cursors :-).
        ... we shall see some other day ...
        */
        template<size_t sz>
        struct fileProcessedHolder {
            std::vector<std::vector<std::string> > data = std::vector<std::vector<std::string>>(sz, std::vector<std::string>());
            size_t cursor = 0;
            int somethingNotFoundAt = -1;

            fileProcessedHolder &operator<<(auto &&toInsert) {
                if (toInsert) data[cursor++].push_back(toInsert.to_string());
                else somethingNotFoundAt = cursor++;
                return *this;
            }

            void resetCursor() { cursor = 0; }
        };

        struct fileProcessedHolder_2 {
            std::vector<std::vector<std::string> > data = std::vector<std::vector<std::string> >();
            bool addLine = true;
            int somethingNotFoundAt = -1;

            fileProcessedHolder_2 &operator<<(auto &&toInsert) {
                if (addLine) {data.push_back(std::vector<std::string>()); addLine = false;}

                if (toInsert) data.back().push_back(toInsert.to_string());
                else somethingNotFoundAt = data.size();
                return *this;
            }
        };

        static auto findNextWithinLine(auto &ctreSrchObj, std::string::iterator &begin,
                                       const std::string::iterator &end) {
            auto result = ctreSrchObj(begin, end);
            if (result) begin = result.get_end_position();
            return result;
        }

    public:
        /*
        It might be a good idea to implement some sort of concept restrictions on typename ctreSrch.
        Might get back to it at some point ... but for my usage this is not really necessary ... will learn Concepts some other day

        Also ... should probably learn how to use exceptions the right way :-)
        */

        template<typename... ctreSrch>
        static std::vector<std::string> processOneLine(std::string &line, ctreSrch &&... perItemInLine) {
            auto bg = line.begin();
            auto end = line.end();
            oneLineProcessedHolder sink;
            (sink << ... << findNextWithinLine(perItemInLine, bg, end));
            return std::move(sink.insideVofS);
        }

        template<typename... ctreSrch>
        static std::vector<std::string> processOneLineRPToneVect(
            std::string &line, ctreSrch &&... perItemInLine) {
            oneLineProcessedHolder sink;

            auto bg = line.begin();
            auto end = line.end();
            do {
                for (int i = 0; i < sizeof...(perItemInLine); ++i) {
                    (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                }
            } while (sink.somethingNotFoundAt == -1);
            return std::move(sink.insideVofS);
        }

        template<typename... ctreSrch>
        static std::vector<std::vector<std::string> >
        processOneLineRPT(std::string &line, ctreSrch &&... perItemInLine) {
            constexpr size_t searchForNumOfItems = sizeof...(perItemInLine);
            fileProcessedHolder<sizeof...(perItemInLine)> sink;

            auto bg = line.begin();
            auto end = line.end();

            while (true) {
                for (int i = 0; i < searchForNumOfItems; ++i) {
                    (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                    sink.resetCursor();
                }
                if (sink.somethingNotFoundAt != -1) break;
            }
            return sink.data;
        }

        template<typename... ctreSrch>
        static std::vector<std::vector<std::string> > processOneLineRPTinFile(
            std::string &dataFile, ctreSrch &&... perItemInLine) {
            std::ifstream iStream;
            iStream.clear();
            iStream.open(dataFile);
            if (not iStream.is_open()) return std::vector<std::vector<std::string> >(
                1, std::vector<std::string>(1, "STREAM NOT OPEN"));

            std::string oneStr;
            std::getline(iStream, oneStr);

            return processOneLineRPT(oneStr, perItemInLine...);
        }

        template<typename... ctreSrch>
        static std::vector<std::vector<std::string> > processFile(std::string &dataFile, ctreSrch &&... perItemInLine) {
            std::ifstream iStream;
            iStream.clear();
            iStream.open(dataFile);
            if (not iStream.is_open()) return std::vector<std::vector<std::string> >(
                1, std::vector<std::string>(1, "STREAM NOT OPEN"));

            fileProcessedHolder<sizeof...(perItemInLine)> sink;
            std::string oneStr;

            while (std::getline(iStream, oneStr)) {
                auto bg = oneStr.begin();
                auto end = oneStr.end();

                (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                sink.resetCursor();
            }
            return sink.data;
        }
        template<typename... ctreSrch>
        static std::vector<std::vector<std::string> > processFileRPT(std::string &dataFile, ctreSrch &&... perItemInLine) {
            std::ifstream iStream;
            iStream.clear();
            iStream.open(dataFile);
            if (not iStream.is_open()) return std::vector<std::vector<std::string> >(
                1, std::vector<std::string>(1, "STREAM NOT OPEN"));

            fileProcessedHolder_2 sink;
            std::string oneStr;
            constexpr size_t searchForNumOfItems = sizeof...(perItemInLine);

            while (std::getline(iStream, oneStr)) {
                auto bg = oneStr.begin();
                auto end = oneStr.end();
                while (true) {
                    for (int i = 0; i < searchForNumOfItems; ++i) {
                        (sink << ... << findNextWithinLine(perItemInLine, bg, end));
                    }
                    if (sink.somethingNotFoundAt != -1) break;
                }
                sink.addLine = true;
                sink.somethingNotFoundAt = -1;
            }
            return sink.data;
        }
    };
}
