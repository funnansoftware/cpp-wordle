#pragma once

#include "Game.hpp"
#include "Render.hpp"

namespace wordle
{
    // The controller layer: turn this frame's raylib keyboard and mouse input into
    // game commands, using the view's layout geometry to hit-test clicks. It drives
    // the Game, but the Game never depends on it.
    auto handle_input(Game& game, const Layout& layout) -> void;
}
