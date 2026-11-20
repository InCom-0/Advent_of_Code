#include <AOC_2022.h>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <cassert>
#include <cstddef>
#include <incom_commons.hpp>
#include <incstd/core/hashing.hpp>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace AOC2022 {
namespace {

using NodeId = std::size_t;

struct FSItem {
    std::string name;
};

struct File : FSItem {
    size_t fileSize{};
};

struct Directory : FSItem {
    NodeId id{};
};

struct DirectoryData {
    std::optional<NodeId>  parent = std::nullopt; // Empty for root
    std::vector<Directory> subdirs{};
    std::vector<File>      files{};

    bool
    isRoot() const {
        return parent.has_value();
    }
};


class FS {
private:
    size_t    nextID{1};
    Directory root{FSItem{"root"}, 0uz};

    ankerl::unordered_dense::map<NodeId, DirectoryData, incstd::hashing::XXH3Hasher> directories{
        {0uz, DirectoryData{}}};

public:
    size_t
    get_nextID() {
        return nextID;
    }

    DirectoryData &
    get(Directory const &dir) {
        return directories.at(dir.id);
    }
    DirectoryData &
    get(NodeId const &dirID) {
        return directories.at(dirID);
    }
    DirectoryData const &
    get(NodeId const &dirID) const {
        return directories.at(dirID);
    }

    DirectoryData &
    add_dirToDir(NodeId const &parentDirID, std::string const &name) {

        auto &contDirData = directories.at(parentDirID);
        contDirData.subdirs.push_back(std::move(Directory{{name}, nextID}));
        auto insRes = directories.insert({nextID++, DirectoryData{.parent = parentDirID}});
        if (not insRes.second) { assert(false); }

        return insRes.first->second;
    }


    File &
    add_fileToDir(NodeId const &parentDirID, File &&toAdd) {
        auto &contDirData = directories.at(parentDirID);
        contDirData.files.push_back(std::move(toAdd));
        return contDirData.files.back();
    }


    std::vector<size_t>
    get_sizesOfDirs() const {
        std::vector<size_t> dirSizes(nextID, std::numeric_limits<size_t>::max());

        auto getSize = [&](this auto const &self, NodeId const &dirID) -> size_t {
            auto       &totSz = dirSizes.at(dirID);
            auto const &dd    = get(dirID);
            totSz = std::ranges::fold_left(dd.files, 0uz,
                                           [](auto const &init, auto const &file) { return init + file.fileSize; });

            for (auto const &containedDir : dd.subdirs) { totSz += self(containedDir.id); }
            return dirSizes.at(dirID);
        };

        getSize(0uz); // It is enough to call it on the root dir
        return dirSizes;
    }
};

} // namespace


auto
day7_0(std::string dataFile) {
    auto any_ctre = ctre::search<R"(.+)">;
    auto d_ctre   = ctre::search<R"(\d+)">;
    auto inp      = incom::aoc::parseInputUsingCTRE::processFile(dataFile, any_ctre).front();

    FS     fileSys;
    NodeId curID{};

    for (size_t lineID = 1uz; lineID < inp.size();) {

        if (inp[lineID].front() == '$') {
            // It is a command
            std::string_view const commandSV(inp[lineID].begin() + 2, inp[lineID].end());

            if (commandSV[0] == 'l' && commandSV[1] == 's') {
                ++lineID;
                while (lineID < inp.size() && inp[lineID].front() != '$') {
                    if (inp[lineID][0] == 'd' && inp[lineID][1] == 'i' && inp[lineID][2] == 'r') {
                        // Its a dir
                        fileSys.add_dirToDir(curID, std::string(inp[lineID].begin() + 4, inp[lineID].end()));
                    }
                    else {
                        auto sizeRes = d_ctre(inp[lineID].begin(), inp[lineID].end());
                        fileSys.add_fileToDir(curID,
                                              File{std::string(sizeRes.get_end_position() + 1, inp[lineID].end()),
                                                   std::stoull(sizeRes.to_string())});
                        // Its a file
                    }
                    ++lineID;
                }
            }
            else if (commandSV[0] == 'c' && commandSV[1] == 'd') {
                std::string_view const target(commandSV.begin() + 3, commandSV.end());
                auto                  &curDirData = fileSys.get(curID);
                if (target[0] == '.' && target[1] == '.') {
                    if (curDirData.parent.has_value()) { curID = curDirData.parent.value(); }
                }
                else {
                    if (auto found = std::ranges::find_if(curDirData.subdirs,
                                                          [&](Directory const &dir) { return dir.name == target; });
                        found != curDirData.subdirs.end()) {
                        curID = found->id;
                    }
                }
                ++lineID;
            }
        }
        else { assert(false); }
    }

    return fileSys;
}
std::size_t
day7_1(std::string dataFile) {

    auto fs       = day7_0(dataFile);
    auto dirSizes = fs.get_sizesOfDirs();
    auto vi       = std::views::filter(dirSizes, [](auto const &sz) { return sz <= 100000; });
    return std::ranges::fold_left(vi, 0uz, std::plus{});
}
std::size_t
day7_2(std::string dataFile) {

    constexpr size_t TotalDiskSpace = 70'000'000;
    constexpr size_t RequiredEmpty  = 30'000'000;

    auto         fs          = day7_0(dataFile);
    auto         dirSizes    = fs.get_sizesOfDirs();
    size_t const needToClear = RequiredEmpty - (TotalDiskSpace - dirSizes.at(0));

    size_t res = std::numeric_limits<size_t>::max();
    for (auto const &dirSz : dirSizes) {
        if (dirSz > needToClear && dirSz < res) { res = dirSz; }
    }

    return res;
}
} // namespace AOC2022