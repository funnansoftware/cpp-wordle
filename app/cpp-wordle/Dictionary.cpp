#include "Dictionary.hpp"

#include <algorithm>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include <wordle/WordList.hpp>

using wordle::Dictionary;

Dictionary::Dictionary(std::size_t word_length)
    : target(word_length, '\0'),
      gen{std::random_device{}()}
{
    const auto parsed = nlohmann::json::parse(wordle::words_json.begin(), wordle::words_json.end());
    words = parsed.get<std::vector<std::string>>();

    if (words.empty())
    {
        throw std::runtime_error{"word list is empty"};
    }

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
