#include "Input.hpp"

#include <raylib.h>

using wordle::Game;
using wordle::Layout;

namespace
{
    // Translate a raylib key code into a Letter. raylib's KEY_A..KEY_Z are the
    // ASCII codes for 'A'..'Z', but an explicit mapping keeps the enum ordering
    // assumption out of the hot path.
    auto key_to_letter(int key) -> wordle::Letter
    {
        using wordle::Letter;

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
                return Letter::None;
        }
    }

    // Route a left-click to the on-screen keyboard or, once the game is over, to
    // the New Game button, using the same geometry the view draws with.
    auto handle_mouse(Game& game, const Layout& layout) -> void
    {
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return;
        }

        const auto mouse = GetMousePosition();

        if (game.is_playing())
        {
            auto y = wordle::keyboard_top_y(layout);
            for (const auto row : wordle::keyboard_layout())
            {
                auto x = wordle::row_start_x(layout, row);
                for (const auto key : row)
                {
                    const auto width = wordle::key_width_for(layout, key);
                    if (CheckCollisionPointRec(mouse, Rectangle{x, y, width, layout.tile_size}))
                    {
                        game.process_letter(key);
                        return;
                    }
                    x += width + layout.spacing;
                }
                y += layout.tile_size + layout.spacing;
            }
        }
        else if (CheckCollisionPointRec(mouse, wordle::new_game_button_rect(layout)))
        {
            game.reset();
        }
    }
}

auto wordle::handle_input(Game& game, const Layout& layout) -> void
{
    // Drain every key queued this frame so fast typing is never dropped.
    for (auto key = GetKeyPressed(); key != KEY_NULL; key = GetKeyPressed())
    {
        game.process_letter(key_to_letter(key));
    }

    handle_mouse(game, layout);
}
