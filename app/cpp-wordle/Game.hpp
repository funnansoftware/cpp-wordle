#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "Dictionary.hpp"
#include "EnumArray.hpp"

namespace wordle
{
    enum class Letter : std::uint8_t
    {
        None,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Enter,
        Delete,
    };

    enum class LetterState : std::uint8_t
    {
        Default,
        Absent,
        Present,
        Correct,
    };

    struct LetterGuess
    {
        Letter letter;
        LetterState state;
    };

    struct Guess
    {
        std::vector<LetterGuess> guesses;
    };

    class Game
    {
    public:
        enum class State
        {
            Playing,
            Won,
            Lost,
        };

        Game(std::size_t max_guesses, const Dictionary& dictionary);

        [[nodiscard]] auto guesses() const -> const std::vector<Guess>&;

        auto process_letter(Letter letter) -> void;

    private:
        auto submit_guess() -> void;
        auto delete_letter() -> void;

        const Dictionary* dictionary{};

        std::vector<Guess> guesses_;
        decltype(std::begin(guesses_)) active_guess;
        std::vector<Letter> target_word;

        EnumArray<Letter, LetterState> keyboard_state;
        State state{State::Playing};
    };
}