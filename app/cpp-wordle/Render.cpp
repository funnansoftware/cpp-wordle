#include "Render.hpp"

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