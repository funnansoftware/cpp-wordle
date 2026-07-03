#include <print>

#include <raylib.h>
#include <chrono>
#include "Dictionary.hpp"
#include "Game.hpp"
#include "Render.hpp"

constexpr auto WindowWidth = 1280;
constexpr auto WindowHeight = 720;
constexpr auto WordLength = 5;
constexpr auto MaxGuesses = 6;
constexpr auto TileSize = 70;
constexpr auto TileSizeF = 70.0F;
constexpr auto Spacing = 5.0F;
constexpr auto FontSize = 32.0F;

auto main() -> int
try
{
    wordle::Dictionary dictionary{WordLength};
    wordle::Game game{MaxGuesses, dictionary};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "Hello, Triangle");
    SetWindowPosition(WindowWidth, static_cast<int>(WindowHeight * 0.5));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        const auto key = GetKeyPressed();
        const auto letter = wordle::raylib_to_letter(key);
        game.process_letter(letter);

        const auto screen_size = Vector2{static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
        const auto board_width = (TileSize * dictionary.target_word().size()) + (Spacing * (dictionary.target_word().size() - 1));

        auto y = Spacing;

        for (const auto& guess : game.guesses())
        {
            auto x = (screen_size.x - board_width) * 0.5F;
            auto tile_rect = Rectangle{x, y, TileSizeF, TileSizeF};

            for (const auto& letter_guess : guess.guesses)
            {
                DrawRectangleLinesEx(tile_rect, 2, GRAY);
                x += TileSizeF + Spacing;

                if (letter_guess.letter != wordle::Letter::None)
                {
                    const auto lg = std::string{magic_enum::enum_name(letter_guess.letter)};
                    const auto text_size = MeasureTextEx(GetFontDefault(), lg.c_str(), FontSize, 1.0F);
                    const auto pos =
                        Vector2{tile_rect.x + (tile_rect.width - text_size.x) * 0.5F, tile_rect.y + (tile_rect.height - text_size.y) * 0.5F};
                    DrawTextEx(GetFontDefault(), lg.c_str(), pos, FontSize, 1.0F, WHITE);
                }

                tile_rect.x = x;
            }

            y += TileSizeF + Spacing;
        }

        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::println("An unexpected error occurred: {}", e.what());
    return EXIT_FAILURE;
}