#include "Game.hpp"

#include <algorithm>
#include <magic_enum/magic_enum.hpp>
#include <ranges>

using wordle::Game;

Game::Game(std::size_t max_guesses, Dictionary& dictionary)
    : dictionary{&dictionary},
      guesses_{max_guesses, {.guesses = std::vector<LetterGuess>(dictionary.word_length(), {Letter::None, LetterState::Default})}},
      active_guess{std::begin(guesses_)}
{
    load_target();
}

auto Game::load_target() -> void
{
    const auto target = dictionary->target_word();

    // One Letter per column, with None for any non-A..Z character, so target_word
    // always has word_length() entries. Scoring indexes it by guess column, so a
    // shorter mapping would read past the end; this keeps positions aligned and in
    // bounds (mirroring the reference's fixed-size letter mapping).
    target_word.assign(dictionary->word_length(), Letter::None);

    for (std::size_t i = 0; i < target.size() && i < target_word.size(); ++i)
    {
        if (const auto letter = magic_enum::enum_cast<Letter>(std::string_view{&target[i], 1}))
        {
            target_word[i] = letter.value();
        }
    }
}

auto Game::reset() -> void
{
    dictionary->reroll();
    load_target();

    for (auto& row : guesses_)
    {
        for (auto& cell : row.guesses)
        {
            cell = {Letter::None, LetterState::Default};
        }
    }

    keyboard_state_ = {};
    active_guess = std::begin(guesses_);
    state = State::Playing;
}

auto Game::guesses() const -> const std::vector<Guess>&
{
    return guesses_;
}

auto Game::messages() const -> const Messages&
{
    return messages_;
}

auto Game::update_messages(std::chrono::duration<float> dt) -> void
{
    messages_.update(dt);
}

auto Game::keyboard_state(Letter letter) const -> LetterState
{
    return keyboard_state_[letter];
}

auto Game::is_playing() const -> bool
{
    return state == State::Playing;
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
    // Reject a guess that is incomplete or is not a real word before scoring it,
    // so the row stays editable and no attempt is spent.
    const auto word = current_word();

    if (word.size() < target_word.size())
    {
        messages_.add("Not enough letters");
        return;
    }

    if (!dictionary->contains(word))
    {
        messages_.add("Not in word list");
        return;
    }

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

    // Promote each key to the best state it has ever shown (Correct beats Present
    // beats Absent), so the on-screen keyboard mirrors the grid.
    for (const auto& guess : active_guess->guesses)
    {
        if (guess.state > keyboard_state_[guess.letter])
        {
            keyboard_state_[guess.letter] = guess.state;
        }
    }

    const auto success = std::ranges::all_of(active_guess->guesses, [](const auto& guess) { return guess.state == LetterState::Correct; });

    if (success)
    {
        state = State::Won;
        messages_.add("You won!");
    }
    else if (std::next(active_guess) == std::end(guesses_))
    {
        state = State::Lost;
        messages_.add("You lost!");
    }
    else
    {
        ++active_guess;
    }
}

auto Game::current_word() const -> std::string
{
    std::string word;

    for (const auto& guess : active_guess->guesses)
    {
        if (guess.letter == Letter::None)
        {
            continue;
        }

        word += magic_enum::enum_name(guess.letter);
    }

    return word;
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
