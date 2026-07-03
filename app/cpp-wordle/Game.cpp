#include "Game.hpp"

#include <algorithm>
#include <magic_enum/magic_enum.hpp>
#include <ranges>

using wordle::Game;

Game::Game(std::size_t max_guesses, const Dictionary& dictionary)
    : dictionary{&dictionary},
      guesses_{max_guesses, {.guesses = std::vector<LetterGuess>(dictionary.target_word().size(), {Letter::None, LetterState::Default})}},
      active_guess{std::begin(guesses_)}
{
    for (auto letter : dictionary.target_word())
    {
        auto e = magic_enum::enum_cast<Letter>(std::string_view{&letter, 1});

        if (!e.has_value())
        {
            continue;
        }

        target_word.push_back(e.value());
    }
}

auto Game::guesses() const -> const std::vector<Guess>&
{
    return guesses_;
}

auto Game::process_letter(Letter letter) -> void
{
    if (state != State::Playing or letter == Letter::None)
    {
        return;
    }

    switch (letter)
    {
        case Letter::Enter:
            submit_guess();
            break;

        case Letter::Delete:
            delete_letter();
            break;

        default:
            for (auto& guess : active_guess->guesses)
            {
                if (guess.letter != Letter::None)
                {
                    continue;
                }

                guess.letter = letter;
                break;
            }
            break;
    }
}

auto Game::submit_guess() -> void
{
    // Track the number of appearances of each letter in the target word.
    EnumArray<Letter, int> letter_counts{};

    for (auto letter : target_word)
    {
        letter_counts[letter]++;
    }

    for (auto&& [i, guess] : active_guess->guesses | std::views::enumerate)
    {
        guess.state = LetterState::Absent;

        if (guess.letter == target_word[i])
        {
            guess.state = LetterState::Correct;
            letter_counts[guess.letter]--;
        }
    }

    // Begin word validation.
    for (auto& guess : active_guess->guesses)
    {
        if (guess.state == LetterState::Correct)
        {
            continue;
        }

        if (letter_counts[guess.letter] > 0)
        {
            guess.state = LetterState::Present;
            letter_counts[guess.letter]--;
        }
    }

    const auto success = std::ranges::all_of(active_guess->guesses, [](const auto& guess) { return guess.state == LetterState::Correct; });

    if (success)
    {
        state = State::Won;
    }
    else if (std::next(active_guess) == std::end(guesses_))
    {
        state = State::Lost;
    }
    else
    {
        ++active_guess;
    }
}

auto Game::delete_letter() -> void
{
    for (auto& guess : active_guess->guesses | std::views::reverse)
    {
        if (guess.letter != Letter::None)
        {
            guess.letter = Letter::None;
            break;
        }
    }
}
