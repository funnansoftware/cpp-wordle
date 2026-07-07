#include "Dictionary.hpp"

#include <algorithm>
#include <random>

using wordle::Dictionary;

Dictionary::Dictionary(std::size_t word_length)
    : words{
          "APPLE", "BANJO", "CRANE", "DELTA", "EAGLE", "FABLE", "MEETS", "PEEVE", "QUILT",
      },
      target(word_length, '\0'),
      gen{std::random_device{}()}
{
    reroll();
}

auto Dictionary::target_word() const -> std::string_view
{
    return target;
}

auto Dictionary::word_length() const -> std::size_t
{
    return target.size();
}

auto Dictionary::contains(std::string_view word) const -> bool
{
    return std::ranges::find(words, word) != words.end();
}

auto Dictionary::reroll() -> void
{
    std::uniform_int_distribution<std::size_t> dist(0, words.size() - 1);
    target = words[dist(gen)];
}
