#pragma once

#include <cstddef>
#include <span>

#include <raylib.h>

#include "Game.hpp"

namespace wordle
{
    // Geometry the board and keyboard are laid out with. Filled per frame from the
    // window size so everything re-centres when the window is resized.
    struct Layout
    {
        float tile_size = 70.0F;
        float spacing = 5.0F;
        Vector2 screen_size{};
        std::size_t word_length = 5;
        std::size_t rows = 6;
    };

    // One row of the on-screen keyboard.
    using KeyRow = std::span<const Letter>;

    // The three QWERTY rows (10/9/9), with Enter and Delete at the ends of the
    // last row. Shared with the input controller for click hit-testing.
    [[nodiscard]] auto keyboard_layout() -> std::span<const KeyRow>;

    // Total width of the guess grid: every tile plus the gaps between them.
    [[nodiscard]] auto board_width(const Layout& layout) -> float;

    // --- Keyboard geometry (shared with the input controller) ---

    // Width of one key; Enter and Delete are wider so their labels fit.
    [[nodiscard]] auto key_width_for(const Layout& layout, Letter key) -> float;

    // Y of the top keyboard row, anchored to the bottom of the screen.
    [[nodiscard]] auto keyboard_top_y(const Layout& layout) -> float;

    // X of a keyboard row's first key, so the row is horizontally centred.
    [[nodiscard]] auto row_start_x(const Layout& layout, KeyRow row) -> float;

    // The "New Game" button rectangle, just below the grid. Shared with the input
    // controller so a click can be routed to it.
    [[nodiscard]] auto new_game_button_rect(const Layout& layout) -> Rectangle;

    // Load the embedded TTF used for all text. Call once after the window (and
    // therefore the GL context) exists; unload_font() before the window closes.
    auto load_font() -> void;
    auto unload_font() -> void;

    // Draw the whole game for one frame. Reads the Game; never mutates it.
    auto render(const Game& game, const Layout& layout) -> void;
}
