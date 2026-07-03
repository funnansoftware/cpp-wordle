#pragma once

#include <raylib.h>
#include "Game.hpp"


namespace wordle
{
    auto raylib_to_letter(int key) -> Letter;
}