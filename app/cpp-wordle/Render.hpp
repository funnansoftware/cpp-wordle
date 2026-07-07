#pragma once

#include <cstddef>

#include <raylib.h>

#include "Game.hpp"

namespace wordle
{
    // Geometry the board is laid out with. Filled per frame from the window size
    // so everything re-centres when the window is resized.
    struct Layout
    {
        float tile_size = 70.0F;
        float spacing = 5.0F;
        Vector2 screen_size{};
        std::size_t word_length = 5;
        std::size_t rows = 6;
    };

    // Total width of the guess grid: every tile plus the gaps between them.
    [[nodiscard]] auto board_width(const Layout& layout) -> float;

    // Draw the whole game for one frame. Reads the Game; never mutates it.
    auto render(const Game& game, const Layout& layout) -> void;

    // Translate a raylib key code into a Letter (belongs with input, kept here
    // until the input controller is split out).
    [[nodiscard]] auto raylib_to_letter(int key) -> Letter;
}
