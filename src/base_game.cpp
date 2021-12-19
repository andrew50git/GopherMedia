#include "base_game.h"

void BaseGame::ProcessInput() {
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running = false;
            break;
        }
    }
    mouse_buttons = SDL_GetGlobalMouseState(&mouse_position.x, &mouse_position.y);
}

bool BaseGame::GetKeyboardState(int keycode) {
    return keys[SDL_GetScancodeFromKey(keycode)];
}

Vector2I BaseGame::GetMousePosition() {
    Vector2I window_position;
    SDL_GetWindowPosition(window, &window_position.x, &window_position.y);
    return {mouse_position.x - window_position.x, mouse_position.y - window_position.y};
}

bool BaseGame::GetMouseButton(MouseButton button) {
    if (button == left) {
        return mouse_buttons & SDL_BUTTON_LMASK;
    } else if (button == middle) {
        return mouse_buttons & SDL_BUTTON_MMASK;
    } else {
        return mouse_buttons & SDL_BUTTON_RMASK;
    }
}

bool BaseGame::GetSuccess() {
    return initialized;
}

void BaseGame::StopLoop() {
    is_running = false;
}

bool BaseGame::GetIsRunning() {
    return is_running;
}