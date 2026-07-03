#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace wordle
{
    class Dictionary
    {
    public:
        Dictionary(std::size_t word_length = 5);

        auto target_word() const -> std::string_view;

    private:
        std::vector<std::string> words;
        std::string target;
    };
}