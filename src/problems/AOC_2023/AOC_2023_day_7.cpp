#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <ctre.hpp>
#include <flux.hpp>
#include <functional>
#include <incom_commons.h>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>


namespace AOC2023 {


enum class HandType : std::size_t {
    none = 0uz,
    high_card,
    one_pair,
    two_pair,
    tripple,
    full_house,
    four_kind,
    five_kind
};


enum class CardsOriginal : std::size_t {
    _2 = 0uz,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _T,
    _J,
    _Q,
    _K,
    _A,
};
enum class CardsUpdated : std::size_t {
    _J = 0uz,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _T,
    _Q,
    _K,
    _A,
};


template <typename CARDS_T>
struct Hand {
public:
    std::array<CARDS_T, 5> cards;
    HandType               type{};

protected:
    Hand(std::array<CARDS_T, 5> cardsIn, HandType typeIn)
        : cards(std::move(cardsIn)), type(typeIn) {}

    static std::array<CARDS_T, 5>
    parseCards(std::string_view sv) {
        assert(sv.size() == 5);
        std::array<CARDS_T, 5> parsed{};
        for (std::size_t id = 0uz; char const c : sv) {
            switch (c) {
                case '2': parsed[id] = CARDS_T::_2; break;
                case '3': parsed[id] = CARDS_T::_3; break;
                case '4': parsed[id] = CARDS_T::_4; break;
                case '5': parsed[id] = CARDS_T::_5; break;
                case '6': parsed[id] = CARDS_T::_6; break;
                case '7': parsed[id] = CARDS_T::_7; break;
                case '8': parsed[id] = CARDS_T::_8; break;
                case '9': parsed[id] = CARDS_T::_9; break;
                case 'T': parsed[id] = CARDS_T::_T; break;
                case 'J': parsed[id] = CARDS_T::_J; break;
                case 'Q': parsed[id] = CARDS_T::_Q; break;
                case 'K': parsed[id] = CARDS_T::_K; break;
                case 'A': parsed[id] = CARDS_T::_A; break;
                default:  assert(false);
            }
            ++id;
        }
        return parsed;
    }

    static HandType
    parseType(std::array<CARDS_T, 5> const &cardsIn) {
        std::array<std::uint8_t, 13> cardCounts{};
        for (CARDS_T const card : cardsIn) { ++cardCounts[std::to_underlying(card)]; }

        std::ranges::sort(cardCounts, std::ranges::greater{});
        switch (cardCounts[0]) {
            case 5:  return HandType::five_kind;
            case 4:  return HandType::four_kind;
            case 3:  return cardCounts[1] == 2 ? HandType::full_house : HandType::tripple;
            case 2:  return cardCounts[1] == 2 ? HandType::two_pair : HandType::one_pair;
            case 1:  return HandType::high_card;
            default: std::unreachable();
        }
    }

public:
    Hand(std::string_view sv)
        : cards(parseCards(sv)), type(parseType(cards)) {}

    bool
    compare(Hand const &other) const {
        Hand const &thisRef = *this;
        if (thisRef.type < other.type) { return true; }
        else if (thisRef.type > other.type) { return false; }

        // Equal in type, need to compare
        else {
            size_t id = 0uz;
            while (id < 5 && (thisRef.cards[id] == other.cards[id])) { ++id; }
            if (thisRef.cards[id] < other.cards[id]) { return true; }
            else { return false; }
        }
        std::unreachable();
    }

    bool
    operator<(Hand const &other) const {
        Hand const &thisRef = *this;
        if (thisRef.type < other.type) { return true; }
        else if (thisRef.type > other.type) { return false; }

        // Equal in type, need to compare
        else {
            size_t id = 0uz;
            while (id < 5 && (thisRef.cards[id] == other.cards[id])) { ++id; }
            if (thisRef.cards[id] < other.cards[id]) { return true; }
            else { return false; }
        }
        std::unreachable();
    }

    bool
    operator>(Hand const &other) const {
        Hand const &thisRef = *this;
        if (thisRef.type > other.type) { return true; }
        else if (thisRef.type < other.type) { return false; }

        // Equal in type, need to compare
        else {
            size_t id = 0uz;
            while (id < 5 && (thisRef.cards[id] == other.cards[id])) { ++id; }
            if (thisRef.cards[id] > other.cards[id]) { return true; }
            else { return false; }
        }
        std::unreachable();
    }

    static bool
    compare(Hand const &a, Hand const &b) {
        return a.compare(b);
    }
};

template <typename CARDS_T>
struct HandUpdated : Hand<CARDS_T> {
private:
    static HandType
    parseTypeWithJokers(std::array<CARDS_T, 5> const &cardsIn) {
        static_assert(std::is_same_v<CARDS_T, CardsUpdated>);
        std::array<std::uint8_t, 13> cardCounts{};
        std::uint8_t                 jokers = 0;

        for (CARDS_T const card : cardsIn) {
            if (card == CardsUpdated::_J) { ++jokers; }
            else { ++cardCounts[std::to_underlying(card)]; }
        }

        std::ranges::sort(cardCounts, std::ranges::greater{});
        cardCounts[0] += jokers;

        switch (cardCounts[0]) {
            case 5:  return HandType::five_kind;
            case 4:  return HandType::four_kind;
            case 3:  return cardCounts[1] == 2 ? HandType::full_house : HandType::tripple;
            case 2:  return cardCounts[1] == 2 ? HandType::two_pair : HandType::one_pair;
            case 1:  return HandType::high_card;
            default: std::unreachable();
        }
    }
    HandUpdated(std::array<CARDS_T, 5> const &cards)
        : Hand<CARDS_T>(cards, parseTypeWithJokers(cards)) {}

public:
    HandUpdated(std::string_view sv)
        : HandUpdated(Hand<CARDS_T>::parseCards(sv)) {}
};


size_t
day7_1(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\w+)">;
    auto input  = std::vector(std::from_range, incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre) |
                                                   std::views::transform([](auto const &twoStrings) {
                                                      if (twoStrings.size() < 2) { assert(false); }
                                                      return std::make_pair(Hand<CardsOriginal>(twoStrings[0]),
                                                                            std::stoull(twoStrings[1]));
                                                   }));

    std::ranges::sort(input, [](auto const &a, auto const &b) { return a.first < b.first; });
    return std::ranges::fold_left(
        std::views::transform(input, [id = 1uz](auto const &prItem) mutable { return prItem.second * (id++); }), 0uz,
        [](auto init, auto const &item) { return init + item; });
}


size_t
day7_2(std::string dataFile) {
    auto d_ctre = ctre::search<R"(\w+)">;
    auto input  = std::vector(std::from_range, incom::aoc::parseInputUsingCTRE::processFileRPT(dataFile, d_ctre) |
                                                   std::views::transform([](auto const &twoStrings) {
                                                      if (twoStrings.size() < 2) { assert(false); }
                                                      return std::make_pair(HandUpdated<CardsUpdated>(twoStrings[0]),
                                                                            std::stoull(twoStrings[1]));
                                                   }));

    std::ranges::sort(input, [](auto const &a, auto const &b) { return a.first < b.first; });
    return std::ranges::fold_left(
        std::views::transform(input, [id = 1uz](auto const &prItem) mutable { return prItem.second * (id++); }), 0uz,
        [](auto init, auto const &item) { return init + item; });
}

} // namespace AOC2023