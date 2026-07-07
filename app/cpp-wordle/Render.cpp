#include "Render.hpp"

#include <string>
#include <string_view>

#include <magic_enum/magic_enum.hpp>

using wordle::Layout;

namespace
{
    // raylib's default font letter spacing.
    constexpr auto TextSpacing = 1.0F;

    // Line thickness of an empty tile's outline.
    constexpr auto GridOutline = 2.0F;

    // The Wordle palette, keyed by letter state.
    constexpr auto letter_color(wordle::LetterState state) -> Color
    {
        switch (state)
        {
            case wordle::LetterState::Absent:
                return Color{58, 58, 60, 255};
            case wordle::LetterState::Present:
                return Color{181, 159, 59, 255};
            case wordle::LetterState::Correct:
                return Color{83, 141, 78, 255};
            case wordle::LetterState::Default:
            default:
                return Color{130, 131, 135, 255};
        }
    }

    // Draw text centred within rect at the given size.
    auto draw_text_centered(std::string_view text, Rectangle rect, float font_size, Color color) -> void
    {
        const std::string owned{text};
        const auto size = MeasureTextEx(GetFontDefault(), owned.c_str(), font_size, TextSpacing);
        const auto pos = Vector2{
            rect.x + (rect.width - size.x) * 0.5F,
            rect.y + (rect.height - size.y) * 0.5F,
        };
        DrawTextEx(GetFontDefault(), owned.c_str(), pos, font_size, TextSpacing, color);
    }

    // Draw the six-row guess grid, tinting each tile by its letter state.
    auto render_board(const wordle::Game& game, const Layout& layout) -> void
    {
        const auto width = wordle::board_width(layout);

        auto y = layout.spacing;
        for (const auto& row : game.guesses())
        {
            auto x = (layout.screen_size.x - width) * 0.5F;
            for (const auto& cell : row.guesses)
            {
                const auto tile = Rectangle{x, y, layout.tile_size, layout.tile_size};

                if (cell.state == wordle::LetterState::Default)
                {
                    DrawRectangleLinesEx(tile, GridOutline, letter_color(wordle::LetterState::Absent));
                }
                else
                {
                    DrawRectangleRec(tile, letter_color(cell.state));
                }

                if (cell.letter != wordle::Letter::None)
                {
                    draw_text_centered(magic_enum::enum_name(cell.letter), tile, layout.tile_size, WHITE);
                }

                x += layout.tile_size + layout.spacing;
            }
            y += layout.tile_size + layout.spacing;
        }
    }
}

auto wordle::board_width(const Layout& layout) -> float
{
    return layout.tile_size * static_cast<float>(layout.word_length) +
           layout.spacing * static_cast<float>(layout.word_length - 1);
}

auto wordle::render(const Game& game, const Layout& layout) -> void
{
    render_board(game, layout);
}

auto wordle::raylib_to_letter(int key) -> Letter
{
    switch (key)
    {
        case KEY_A:
            return Letter::A;
        case KEY_B:
            return Letter::B;
        case KEY_C:
            return Letter::C;
        case KEY_D:
            return Letter::D;
        case KEY_E:
            return Letter::E;
        case KEY_F:
            return Letter::F;
        case KEY_G:
            return Letter::G;
        case KEY_H:
            return Letter::H;
        case KEY_I:
            return Letter::I;
        case KEY_J:
            return Letter::J;
        case KEY_K:
            return Letter::K;
        case KEY_L:
            return Letter::L;
        case KEY_M:
            return Letter::M;
        case KEY_N:
            return Letter::N;
        case KEY_O:
            return Letter::O;
        case KEY_P:
            return Letter::P;
        case KEY_Q:
            return Letter::Q;
        case KEY_R:
            return Letter::R;
        case KEY_S:
            return Letter::S;
        case KEY_T:
            return Letter::T;
        case KEY_U:
            return Letter::U;
        case KEY_V:
            return Letter::V;
        case KEY_W:
            return Letter::W;
        case KEY_X:
            return Letter::X;
        case KEY_Y:
            return Letter::Y;
        case KEY_Z:
            return Letter::Z;
        case KEY_ENTER:
            return Letter::Enter;
        case KEY_BACKSPACE:
            return Letter::Delete;
        default:
            break;
    }

    return Letter::None;
}
