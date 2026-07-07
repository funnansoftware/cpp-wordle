#include "Render.hpp"

#include <array>
#include <span>
#include <string>
#include <string_view>

#include <magic_enum/magic_enum.hpp>

using wordle::KeyRow;
using wordle::Layout;

namespace
{
    // raylib's default font letter spacing.
    constexpr auto TextSpacing = 1.0F;

    // Line thickness of an empty tile's outline.
    constexpr auto GridOutline = 2.0F;

    // Letter keys are a little narrower than a grid tile.
    constexpr auto KeyWidthRatio = 0.65F;

    // Enter and Delete spell a word rather than a glyph, so they get a wider key.
    constexpr auto WideKeyScale = 1.5F;

    // Font size for UI text (button label and message toasts).
    constexpr auto UiFontSize = 24.0F;

    // "New Game" button dimensions and outline thickness.
    constexpr auto ButtonWidth = 150.0F;
    constexpr auto ButtonHeight = 50.0F;
    constexpr auto ButtonOutline = 4.0F;

    // The fixed QWERTY arrangement. Rows are ragged (10/9/9).
    constexpr std::array KeyboardTop{
        wordle::Letter::Q, wordle::Letter::W, wordle::Letter::E, wordle::Letter::R, wordle::Letter::T,
        wordle::Letter::Y, wordle::Letter::U, wordle::Letter::I, wordle::Letter::O, wordle::Letter::P};
    constexpr std::array KeyboardHome{
        wordle::Letter::A, wordle::Letter::S, wordle::Letter::D, wordle::Letter::F, wordle::Letter::G,
        wordle::Letter::H, wordle::Letter::J, wordle::Letter::K, wordle::Letter::L};
    constexpr std::array KeyboardBottom{
        wordle::Letter::Enter, wordle::Letter::Z, wordle::Letter::X, wordle::Letter::C, wordle::Letter::V,
        wordle::Letter::B, wordle::Letter::N, wordle::Letter::M, wordle::Letter::Delete};
    constexpr std::array<KeyRow, 3> KeyboardRows{KeyboardTop, KeyboardHome, KeyboardBottom};

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

    // Like draw_text_centered, but shrinks the font so the label fits the rect's
    // width. Single glyphs render as-is; "Enter"/"Delete" scale down.
    auto draw_text_fit(std::string_view text, Rectangle rect, float font_size, Color color) -> void
    {
        constexpr auto TextPadding = 8.0F;

        const std::string owned{text};
        auto size = font_size;
        const auto max_width = rect.width - TextPadding * 2.0F;
        const auto text_width = MeasureTextEx(GetFontDefault(), owned.c_str(), size, TextSpacing).x;
        if (text_width > max_width && text_width > 0.0F)
        {
            size *= max_width / text_width;
        }

        draw_text_centered(text, rect, size, color);
    }

    // Base letter-key width; every key derives from this.
    auto base_key_width(const Layout& layout) -> float
    {
        return layout.tile_size * KeyWidthRatio;
    }

    // Total laid-out width of a row: every key plus the gaps between them.
    auto row_width(const Layout& layout, KeyRow row) -> float
    {
        auto width = layout.spacing * static_cast<float>(row.size() - 1);
        for (const auto key : row)
        {
            width += wordle::key_width_for(layout, key);
        }
        return width;
    }

    // Draw the on-screen keyboard, tinting each key by its best seen state.
    auto render_keyboard(const wordle::Game& game, const Layout& layout) -> void
    {
        auto y = wordle::keyboard_top_y(layout);
        for (const auto row : wordle::keyboard_layout())
        {
            auto x = wordle::row_start_x(layout, row);
            for (const auto key : row)
            {
                const auto width = wordle::key_width_for(layout, key);
                const auto rect = Rectangle{x, y, width, layout.tile_size};

                DrawRectangleRec(rect, letter_color(game.keyboard_state(key)));
                draw_text_fit(magic_enum::enum_name(key), rect, layout.tile_size * 0.5F, WHITE);

                x += width + layout.spacing;
            }
            y += layout.tile_size + layout.spacing;
        }
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

    // Draw the "New Game" button shown once the game is over.
    auto render_new_game_button(const Layout& layout) -> void
    {
        const auto rect = wordle::new_game_button_rect(layout);
        DrawRectangleLinesEx(rect, ButtonOutline, WHITE);
        draw_text_centered("New Game", rect, UiFontSize, WHITE);
    }

    // Stack the active message toasts (white boxes, black text) near the top.
    auto render_messages(const wordle::Game& game, const Layout& layout) -> void
    {
        constexpr auto MessagePadding = 10.0F;

        auto y = layout.spacing * 2.0F;
        for (const auto& message : game.messages().all())
        {
            const std::string owned{message.text};
            const auto text_size = MeasureTextEx(GetFontDefault(), owned.c_str(), UiFontSize, TextSpacing);
            const auto width = text_size.x + MessagePadding * 2.0F;
            const auto height = text_size.y + MessagePadding * 2.0F;
            const auto rect = Rectangle{(layout.screen_size.x - width) * 0.5F, y, width, height};

            DrawRectangleRec(rect, WHITE);
            draw_text_centered(message.text, rect, UiFontSize, BLACK);

            y += height + layout.spacing * 2.0F;
        }
    }
}

auto wordle::board_width(const Layout& layout) -> float
{
    return layout.tile_size * static_cast<float>(layout.word_length) +
           layout.spacing * static_cast<float>(layout.word_length - 1);
}

auto wordle::keyboard_layout() -> std::span<const KeyRow>
{
    return KeyboardRows;
}

auto wordle::key_width_for(const Layout& layout, Letter key) -> float
{
    auto width = base_key_width(layout);
    if (key == Letter::Enter || key == Letter::Delete)
    {
        width *= WideKeyScale;
    }
    return width;
}

auto wordle::keyboard_top_y(const Layout& layout) -> float
{
    // Three rows tall, anchored to the bottom of the screen.
    return layout.screen_size.y - (layout.tile_size + layout.spacing) * 3.0F;
}

auto wordle::row_start_x(const Layout& layout, KeyRow row) -> float
{
    return (layout.screen_size.x - row_width(layout, row)) * 0.5F;
}

auto wordle::new_game_button_rect(const Layout& layout) -> Rectangle
{
    const auto board_height = layout.spacing + static_cast<float>(layout.rows) * (layout.tile_size + layout.spacing);
    return Rectangle{(layout.screen_size.x - ButtonWidth) * 0.5F, board_height, ButtonWidth, ButtonHeight};
}

auto wordle::render(const Game& game, const Layout& layout) -> void
{
    render_board(game, layout);

    if (game.is_playing())
    {
        render_keyboard(game, layout);
    }
    else
    {
        render_new_game_button(layout);
    }

    render_messages(game, layout);
}
