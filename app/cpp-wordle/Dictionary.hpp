#pragma once

#include <cstddef>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace wordle
{
    // The word list plus the current target. Owns nothing platform-specific, so it
    // can be driven and inspected without a window -- the game rules ask it whether
    // a guess is a real word and which word to hunt for.
    class Dictionary
    {
    public:
        Dictionary(std::size_t word_length = 5);

        // The word the player is trying to guess.
        [[nodiscard]] auto target_word() const -> std::string_view;

        // The number of letters every word (and therefore every guess) has.
        [[nodiscard]] auto word_length() const -> std::size_t;

        // Whether word is an accepted guess, i.e. present in the dictionary.
        [[nodiscard]] auto contains(std::string_view word) const -> bool;

        // Choose a fresh random target, e.g. when starting a new game.
        auto reroll() -> void;

    private:
        std::vector<std::string> words;
        std::string target;
        std::mt19937 gen;
    };
}
