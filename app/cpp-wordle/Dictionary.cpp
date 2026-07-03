#include "Dictionary.hpp"

#include <random>

using wordle::Dictionary;

Dictionary::Dictionary(std::size_t word_length)
    : words{
          "APPLE", "BANJO", "CRANE", "DELTA", "EAGLE", "FABLE", "MEETS", "PEEVE", "QUILT",
      },
      target(word_length, '\0')
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, words.size() - 1);
    target = words[dist(gen)];
}

auto Dictionary::target_word() const -> std::string_view
{
    return target;
}