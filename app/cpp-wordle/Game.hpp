#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "Dictionary.hpp"
#include "EnumArray.hpp"
#include "Messages.hpp"

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

        Game(std::size_t max_guesses, Dictionary& dictionary);

        [[nodiscard]] auto guesses() const -> const std::vector<Guess>&;
        [[nodiscard]] auto messages() const -> const Messages&;
        [[nodiscard]] auto keyboard_state(Letter letter) const -> LetterState;
        [[nodiscard]] auto is_playing() const -> bool;

        auto process_letter(Letter letter) -> void;
        auto update_messages(std::chrono::duration<float> dt) -> void;

        // Clear the board and pick a fresh target for another round.
        auto reset() -> void;

    private:
        auto submit_guess() -> void;
        auto delete_letter() -> void;

        // Read the dictionary's current target into target_word as Letters.
        auto load_target() -> void;

        // The active row's filled letters as an uppercase string, for validating
        // against the dictionary.
        [[nodiscard]] auto current_word() const -> std::string;

        Dictionary* dictionary{};

        std::vector<Guess> guesses_;
        decltype(std::begin(guesses_)) active_guess;
        std::vector<Letter> target_word;

        // Best state seen so far for each key, so the on-screen keyboard can tint
        // A..Z. None/Enter/Delete are never written.
        EnumArray<Letter, LetterState> keyboard_state_{};
        State state{State::Playing};
        Messages messages_;
    };
}