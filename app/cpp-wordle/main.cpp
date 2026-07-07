#include <chrono>
#include <print>

#include <raylib.h>

#include "Dictionary.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Render.hpp"

constexpr auto WindowWidth = 1280;
constexpr auto WindowHeight = 720;
constexpr auto WordLength = 5;
constexpr auto MaxGuesses = 6;
constexpr auto TileSize = 70.0F;
constexpr auto Spacing = 5.0F;

auto main() -> int
try
{
    wordle::Dictionary dictionary{WordLength};
    wordle::Game game{MaxGuesses, dictionary};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "Wordle");
    SetWindowPosition(WindowWidth, static_cast<int>(WindowHeight * 0.5));

    auto previous = std::chrono::steady_clock::now();

    while (!WindowShouldClose())
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> dt = now - previous;
        previous = now;

        const auto layout = wordle::Layout{
            .tile_size = TileSize,
            .spacing = Spacing,
            .screen_size = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
            .word_length = dictionary.word_length(),
            .rows = MaxGuesses,
        };

        wordle::handle_input(game, layout);
        game.update_messages(dt);

        BeginDrawing();
        ClearBackground(BLACK);
        wordle::render(game, layout);
        EndDrawing();

        // vcpkg raylib 6 doesn't configure correctly. This means we need to
        // control when to invoke buffer swaps and event polling.
        SwapScreenBuffer();
        PollInputEvents();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::println("An unexpected error occurred: {}", e.what());
    return EXIT_FAILURE;
}
